#ifndef _TINYFSM_H_
#define _TINYFSM_H_
/**
 * @file tinyfsm.h
 * @brief Tiny Finite State Machine (FSM) framework.
 * 
 * @author Edwin Koch
 * @date 10. June 2024
 * 
 * @license MIT License
 * See the LICENSE file in the project root for more information.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef enum{
  FSM_RUNNING
} tfsm_returnState_t;


// forward declaration
typedef struct tfsm_t tfsm_t;

typedef void (tfsm_stateCallback_t)(tfsm_t* fsm);

struct tfsm_t{
  tfsm_stateCallback_t currentState;
  void* contextData;
};

bool tfsm_init(tfsm_t* fsm, tfsm_stateCallback_t entryState, void* contextData);

bool tfsm_routine(tfsm_t* fsm);

void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextState);

#ifdef __cplusplus
}
#endif

#endif // _TINYFSM_H_
