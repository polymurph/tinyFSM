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
    FSM_RUNNING = 1,
    FSM_EXITED = 0,
    FSM_ERROR = -1,
    FSM_INIT_SUCCESSFUL = 2,
    FSM_INIT_FAIL = -2
} tfsm_returnState_t;


// forward declaration
typedef struct tfsm_t tfsm_t;

typedef void (*tfsm_stateCallback_t)(tfsm_t* fsm);

typedef void (*tfsm_mutexLockCallback_t)(void);

struct tfsm_t{
    tfsm_stateCallback_t currentState;
    void* contextData;
    tfsm_mutexLockCallback_t lock;
    tfsm_mutexLockCallback_t unlock;
};

tfsm_returnState_t tfsm_init(
    tfsm_t* fsm,
    tfsm_stateCallback_t entryState,
    void* contextData,
    tfsm_mutexLockCallback_t lock,
    tfsm_mutexLockCallback_t unlock
);

tfsm_returnState_t tfsm_routine(tfsm_t* fsm);

void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextState);

#ifdef __cplusplus
}
#endif

#endif // _TINYFSM_H_
