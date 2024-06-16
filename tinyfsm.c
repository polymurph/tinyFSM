/**
 * @file tinyfsm.c
 * @brief Tiny Finite State Machine (FSM) framework.
 * 
 * @author Edwin Koch
 * @date 10. June 2024
 * 
 * @license MIT License
 * See the LICENSE file in the project root for more information.
 */

#include "tinyfsm.h"

void fsmInit(
    fsm_t* fsmObject,
    fsmStateRoutine_t entryState,
    fsmAction_t entryAction)
{
    fsmObject->entryState = entryState;
    fsmObject->previousState = entryState;
    fsmObject->currentState = entryState;
    fsmObject->nextState = entryState;
    fsmObject->entryAction = entryAction;
    fsmObject->action = entryAction;
    fsmObject->state = STATE_START;
}

fsmStatus_t fsmRun(
    fsm_t* fsmObject)
{
    switch(fsmObject->state){
        case STATE_START:
            fsmObject->entryAction();
            fsmObject->state = STATE_NO_CHANGE;
            return FSM_RUNNING;

        case STATE_NO_CHANGE:
            fsmObject->currentState();
            return FSM_RUNNING;

        case STATE_TRANSITION:
            // change state
            fsmObject->previousState = fsmObject->currentState;
            fsmObject->currentState = fsmObject->nextState;
            fsmObject->action();
            fsmObject->state = STATE_NO_CHANGE;
            return FSM_RUNNING;

        case STATE_END_FSM:
            fsmObject->previousState = fsmObject->entryState;
            fsmObject->currentState = fsmObject->entryState;
            fsmObject->nextState = fsmObject->entryState;
            fsmObject->action();
            fsmObject->state = STATE_NO_CHANGE;
            return FSM_ENDED;
        default:
            // invalid state return
            return FSM_FAULT_UNKNOWN_STATE_RETURN;
    }
    return FSM_RUNNING;
}

void fsmTransitionState(
    fsm_t* fsmObject,
    fsmStateRoutine_t nextState,
    fsmAction_t action)
{
    fsmObject->state = STATE_TRANSITION;
    fsmObject->action = action;
    fsmObject->nextState = nextState;
}

void fsmEndFSM(
    fsm_t* fsmObject,
    fsmAction_t fsmEndingAction)
{
    fsmObject->action = fsmEndingAction;
    fsmObject->state = STATE_END_FSM;
}

void fsmNoAction()
{

}
