/**
 * @file tinyfsm.c
 * @brief Tiny Finite State Machine (FSM) framework.
 * 
 * @authored by Edwin Koch
 * @date 10. June 2024
 * 
 * @license MIT License
 * See the LICENSE file in the project root for more information.
 */

#include "tinyfsm.h"

// No-op lock function for single-threaded use
static unsigned int _singleThreadedLock(fsmMutex_t mutexObject) {
    return 0;  // Success
}

// No-op unlock function for single-threaded use
static unsigned int _singleThreadedUnlock(fsmMutex_t mutexObject) {
    return 0;  // Success
}

void fsmInit(
    fsm_t* fsmObject,
    fsmStateRoutine_t entryState,
    fsmAction_t entryAction,
    fsmAction_t fsmEndingAction,
    fsmMutex_t mutexObject,
    fsmMutexOps_t mutexOps)
{
    fsmObject->entryState = entryState;
    fsmObject->previousState = entryState;
    fsmObject->currentState = entryState;
    fsmObject->nextState = entryState;
    fsmObject->entryAction = entryAction;
    fsmObject->fsmEndingAction = fsmEndingAction;
    fsmObject->action = entryAction;
    fsmObject->state = STATE_START;
    fsmObject->mutexObject = mutexObject;
    fsmObject->mutexOps = mutexOps;
    fsmObject->initialized = 1;
}

void fsmInitSingleThreaded(
    fsm_t* fsmObject,
    fsmStateRoutine_t entryState,
    fsmAction_t entryAction,
    fsmAction_t fsmEndingAction)
{
    static fsmMutexOps_t singleThreadedOps = {
        .lock = _singleThreadedLock,
        .unlock = _singleThreadedUnlock
    };

    fsmObject->entryState = entryState;
    fsmObject->previousState = entryState;
    fsmObject->currentState = entryState;
    fsmObject->nextState = entryState;
    fsmObject->entryAction = entryAction;
    fsmObject->fsmEndingAction = fsmEndingAction;
    fsmObject->action = entryAction;
    fsmObject->state = STATE_START;
    fsmObject->mutexObject = 0;
    fsmObject->mutexOps = singleThreadedOps;
    fsmObject->initialized = 1;
}

fsmStatus_t fsmRun(fsm_t* fsmObject)
{
	// cease execution if not initialized
    if (fsmObject->initialized != 1) return FSM_NOT_INITIALIZED;

    // mutex lock
    if (fsmObject->mutexOps.lock(fsmObject->mutexObject) != 0) return FSM_MUTEX_LOCKED;

    switch (fsmObject->state) {
        case STATE_START:
            fsmObject->entryAction();
            fsmObject->state = STATE_NO_CHANGE;
            break;

        case STATE_NO_CHANGE:
        	fsmObject->mutexOps.unlock(fsmObject->mutexObject);
            fsmObject->currentState();
        	// wait until mutex is unlocked and then lock it
        	while(fsmObject->mutexOps.lock(fsmObject->mutexObject) != 0);
            break;

        case STATE_TRANSITION:
            // Change state
            fsmObject->previousState = fsmObject->currentState;
            fsmObject->currentState = fsmObject->nextState;
            fsmObject->action();
            fsmObject->state = STATE_NO_CHANGE;
            break;

        case STATE_END_FSM:
            // End the finite state machine
            fsmObject->previousState = fsmObject->entryState;
            fsmObject->currentState = fsmObject->entryState;
            fsmObject->nextState = fsmObject->entryState;
            fsmObject->fsmEndingAction();
            fsmObject->state = STATE_NO_CHANGE;

            // mutex unlock
            fsmObject->mutexOps.unlock(fsmObject->mutexObject);
            return FSM_ENDED;

        default:
            // Invalid state
            fsmObject->mutexOps.unlock(fsmObject->mutexObject);
            return FSM_FAULT_UNKNOWN_STATE_RETURN;
    }

    // mutex unlock
    fsmObject->mutexOps.unlock(fsmObject->mutexObject);
    return FSM_RUNNING;
}

void fsmTransitionState(
    fsm_t* fsmObject,
    fsmStateRoutine_t nextState,
    fsmAction_t action)
{
	// cease execution if not initialized
    if (fsmObject->initialized != 1) return;

    // mutex lock
    if (fsmObject->mutexOps.lock(fsmObject->mutexObject) != 0) return;

    fsmObject->state = STATE_TRANSITION;
    fsmObject->action = action;
    fsmObject->nextState = nextState;

    // mutex unlock
    fsmObject->mutexOps.unlock(fsmObject->mutexObject);
}

void fsmEndFSM(fsm_t* fsmObject)
{
	// cease execution if not initialized
    if (fsmObject->initialized != 1) return;

    // mutex lock
    if (fsmObject->mutexOps.lock(fsmObject->mutexObject) != 0) return;

    fsmObject->state = STATE_END_FSM;

    // mutex unlock
    fsmObject->mutexOps.unlock(fsmObject->mutexObject);
}

void fsmNoAction()
{
    // No action function
}
