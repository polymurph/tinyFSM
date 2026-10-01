#include "tfsm.h"

bool tfsm_init(tfsm_t* fsm, tfsm_state_t entryState, void* contextData)
{
  if(fsm == NULL) return false;

  fsm->currentState = entryState;
  fsm->contextData = contextData;
  return true;
}

bool tfsm_routine(tfsm_t* fsm)
{
  if(fsm == NULL) return false;

  if(fsm->currentState.callback == tfsm_end || fsm->currentState.callback == NULL) return false;
  
  fsm->currentState = fsm->currentState.callback(fsm);
  
  return true;
}

tfsm_state_t tfsm_end(tfsm_t* fsm)
{
  return (tfsm_state_t){NULL};
}

