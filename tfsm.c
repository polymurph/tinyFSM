#include "tfsm.h"

static inline void _lock(tfsm_t* fsm);
static inline void _unlock(tfsm_t* fsm);

tfsm_returnState_t tfsm_init(tfsm_t* fsm, tfsm_stateCallback_t entryState, void* contextData, tfsm_mutexLockCallback_t lock, tfsm_mutexLockCallback_t unlock)
{
    if(fsm == NULL) return FSM_INIT_FAIL;

    if(lock != NULL && unlock != NULL){
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
    if(fsm == NULL) return FSM_ERROR;

    _lock(fsm);
    tfsm_stateCallback_t currentState = fsm->currentState;
    _unlock(fsm);
    
    if(currentState == NULL) return FSM_EXITED;

    currentState(fsm);

    return FSM_RUNNING;
}

void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextStateCallback)
{
    if(fsm == NULL) return;

    _lock(fsm);
    fsm->currentState = nextStateCallback;
    _unlock(fsm);
}

static inline void _lock(tfsm_t* fsm)
{
    if(fsm->lock != NULL) fsm->lock();
}

static inline void _unlock(tfsm_t* fsm)
{

    if(fsm->unlock != NULL) fsm->unlock();
}

