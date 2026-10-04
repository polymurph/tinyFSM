#ifndef _TINYFSM_H_
#define _TINYFSM_H_

/**
 * @file tinyfsm.h
 * @brief Tiny, thread-safe Finite State Machine (FSM) framework for embedded and RTOS applications.
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

/**
 * @brief Return states for the FSM execution routine and initialization.
 */
typedef enum {
    FSM_RUNNING         = 1,   /**< FSM executed a state successfully and is running. */
    FSM_EXITED          = 0,   /**< FSM reached a termination state (currentState is NULL). */
    FSM_ERROR           = -1,  /**< Invalid argument or null engine pointer error. */
    FSM_INIT_SUCCESSFUL = 2,   /**< FSM instance initialized successfully. */
    FSM_INIT_FAIL       = -2   /**< Initialization failed due to invalid arguments. */
} tfsm_returnState_t;

/* Forward declaration of the FSM context structure. */
typedef struct tfsm_t tfsm_t;

/**
 * @brief Function pointer signature for an FSM state handler function.
 * @param fsm Pointer to the state machine context instance executing the state.
 */
typedef void (*tfsm_stateCallback_t)(tfsm_t* fsm);

/**
 * @brief Function pointer signature for platform-specific mutex locking/unlocking callbacks.
 * @param fsm Pointer to the state machine context instance requesting the lock action.
 */
typedef void (*tfsm_mutexLockCallback_t)(tfsm_t* fsm);

/**
 * @struct tfsm_t
 * @brief Core structure containing the state machine instance context.
 */
struct tfsm_t {
    tfsm_stateCallback_t volatile currentState; /**< Function pointer to the active state callback. Marked volatile for thread safety. */
    void* contextData;                          /**< User-defined pointer to store custom state variables, queues, or hardware contexts. */
    tfsm_mutexLockCallback_t lock;              /**< Callback to lock the platform-specific mutex wrapper. Can be NULL if thread safety is not needed. */
    tfsm_mutexLockCallback_t unlock;            /**< Callback to unlock the platform-specific mutex wrapper. Can be NULL if thread safety is not needed. */
};

/**
 * @brief Initializes a Tiny State Machine instance.
 * 
 * Sets the initial entry state, links user data, and binds the lock/unlock callbacks.
 * 
 * @param[out] fsm           Pointer to the state machine memory allocated by the caller.
 * @param[in]  entryState    The initial state function to run on the first routine call.
 * @param[in]  contextData   Optional pointer to user-defined data structure (can be NULL).
 * @param[in]  lock          Optional callback to lock a normal mutex wrapper (can be NULL).
 * @param[in]  unlock        Optional callback to unlock a normal mutex wrapper (can be NULL).
 * 
 * @return FSM_INIT_SUCCESSFUL on success, or FSM_INIT_FAIL if the fsm pointer is NULL.
 */
tfsm_returnState_t tfsm_init(
    tfsm_t* fsm,
    tfsm_stateCallback_t entryState,
    void* contextData,
    tfsm_mutexLockCallback_t lock,
    tfsm_mutexLockCallback_t unlock
);

/**
 * @brief Executes the active state of the state machine.
 * 
 * Briefly locks the internal mutex to snapshot the current state handler pointer, drops the
 * lock immediately, and then safely runs the state function unlocked. This design eliminates
 * self-transition deadlocks and functions seamlessly with normal, non-recursive mutexes.
 * 
 * @note This function should typically be called inside a worker thread loop or periodic timer loop.
 * 
 * @param[in,out] fsm Pointer to the state machine context instance.
 * 
 * @retval FSM_RUNNING If the current state executed successfully.
 * @retval FSM_EXITED  If the state machine reached a termination state (pointer evaluated to NULL).
 * @retval FSM_ERROR   If the fsm context pointer is invalid (NULL).
 */
tfsm_returnState_t tfsm_routine(tfsm_t* fsm);

/**
 * @brief Transitions the state machine to a new state.
 * 
 * Safely wraps the internal `currentState` pointer swap within the user-provided mutex 
 * lock function to ensure thread safety across concurrent state transitions.
 * 
 * @param[in,out] fsm               Pointer to the state machine context instance.
 * @param[in]     nextStateCallback Function pointer targeting the next state handler to execute.
 */
void tfsm_transitionState(tfsm_t* fsm, tfsm_stateCallback_t nextStateCallback);

#ifdef __cplusplus
}
#endif

#endif // _TINYFSM_H_
