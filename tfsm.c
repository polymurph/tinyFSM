/**
 * @file tfsm.c
 * @brief Source implementation for the Tiny Finite State Machine framework.
 * 
 * @license MIT License
 */

#include "tfsm.h"

/**
 * @brief Internal helper to lock the state machine using its configured callback.
 * @param fsm Pointer to the state machine context.
 */
static inline void _lock(tfsm_t* fsm);

/**
 * @brief Internal helper to unlock the state machine using its configured callback.
 * @param fsm Pointer to the state machine context.
 */
static inline void _unlock(tfsm_t* fsm);

tfsm_returnState_t tfsm_init(tfsm_t* fsm, tfsm_stateCallback_t entryState, void* contextData, tfsm_mutexLockCallback_t lock, tfsm_mutexLockCallback_t unlock)
{
    if (fsm == NULL) return FSM_INIT_FAIL;

    if (lock != NULL && unlock != NULL) {
        fsm->lock = lock;
        fsm->unlock = unlock;
    } else {
        fsm->lock = NULL;
        fsm->unlock = NULL;
    }

    fsm->currentState = entryState;
    fsm->contextData = contextData;
    return FSM_INIT_SUCCESSFUL;
}

tfsm_returnState_t tfsm_routine(tfsm_t* fsm)
{
    if (fsm == NULL) return FSM_ERROR;

    // 1. Lock briefly to snap a copy of the pointer safely
    _lock(fsm);
    tfsm_stateCallback_t currentState = fsm->currentState;
    _unlock(fsm); // 2. Unlock immediately so normal mutexes can be reused safely!

    // 3. Check the local snapshot safely
    if (currentState == NULL) return FSM_EXITED;

    // 4. Run the callback completely unlocked.
    // It can now safely call tfsm_transitionState() using a normal mutex.
    currentState(fsm);

    return FSM_RUNNING;
}

void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextStateCallback)
{
    if (fsm == NULL) return;

    _lock(fsm);
    fsm->currentState = nextStateCallback;
    _unlock(fsm);
}

static inline void _lock(tfsm_t* fsm)
{
    if (fsm->lock != NULL) fsm->lock(fsm);
}

static inline void _unlock(tfsm_t* fsm)
{
    if (fsm->unlock != NULL) fsm->unlock(fsm);
}
