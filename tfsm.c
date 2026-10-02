#include "tfsm.h"

bool tfsm_init(tfsm_t* fsm, tfsm_stateCallback_t entryState, void* contextData)
{
  if(fsm == NULL) return false;

  fsm->currentState = entryState;
  fsm->userData = contextData;
  return true;
}

bool tfsm_routine(tfsm_t* fsm)
{
  if(fsm == NULL) return false;

  fsm->currentState(fsm);
  
  return true;
}

void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextStateCallback)
{
  fsm->currentState = nextStateCallback;
}

