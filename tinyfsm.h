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

/**
 * @brief Typedef for FSM action function pointer.
 */
typedef void (*fsmAction_t)(void);

/**
 * @brief Typedef for mutex object.
 */
typedef void* fsmMutex_t;

/**
 * @ brief Mutex operations structure definition
 */
typedef struct{
	unsigned int (*lock)(fsmMutex_t);		/**< mutex lock function pointer */
	unsigned int (*unlock)(fsmMutex_t);		/**< mutex unlock function pointer */
}fsmMutexOps_t;

/**
 * @brief FSM status codes.
 */
typedef enum {
    FSM_RUNNING,                     /**< FSM is running */
    FSM_ENDED,                       /**< FSM has ended */
	FSM_NOT_INITIALIZED,			 /**< FSM is ot initialized */
	FSM_MUTEX_LOCKED,				 /**< FSM mutex is locked */
    FSM_FAULT_UNKNOWN_STATE_RETURN   /**< FSM encountered an unknown state return */
} fsmStatus_t;

/**
 * @brief FSM state semaphore codes.
 */
typedef enum {
    STATE_START,                     /**< FSM is in start state */
    STATE_TRANSITION,                /**< FSM is in transition state */
    STATE_NO_CHANGE,                 /**< FSM state has no change */
    STATE_END_FSM                    /**< FSM has ended */
} fsmStateSemaphore_t;

/**
 * @brief Typedef for FSM state routine function pointer.
 */
typedef void (*fsmStateRoutine_t)(void);

/**
 * @brief FSM structure definition.
 */
typedef struct {
    fsmStateRoutine_t entryState;    /**< Entry state routine */
    fsmAction_t entryAction;         /**< Entry action */
    fsmAction_t fsmEndingAction;	 /**< FSM ending action */
    fsmStateRoutine_t previousState; /**< Previous state routine */
    fsmStateRoutine_t currentState;  /**< Current state routine */
    fsmStateRoutine_t nextState;     /**< Next state routine */
    fsmStateSemaphore_t state;       /**< Current state semaphore */
    fsmAction_t action;              /**< Current action */
    fsmMutex_t mutexObject;			 /**< Pointer to mutex object */
    fsmMutexOps_t mutexOps;			 /**< Mutex operations */
    unsigned int initialized;		 /**< Flag to indicate if the FSM is initialized. 0 = false and 1 = true */
} fsm_t;

/**
 * @brief Initialize the FSM.
 * 
 * @param fsmObject Pointer to the FSM object.
 * @param entryState Pointer to the entry state routine.
 * @param entryAction Pointer to the entry action function.
 * @param fsmEndingAction Pointer to the FSM endign action to be executed upon ending the FSM.
 * @param mutexObject Mutex Object for thread safety functionality
 * @param mutexOps Mutex operatrions for thread safety functionality
 */
void fsmInit(
    fsm_t* fsmObject,
    fsmStateRoutine_t entryState,
    fsmAction_t entryAction,
	fsmAction_t fsmEndingAction,
	fsmMutex_t mutexObject,
	fsmMutexOps_t mutexOps);

/**
 * @brief Initialize the FSM for single threaded usecase.
 *
 * @param fsmObject Pointer to the FSM object.
 * @param entryState Pointer to the entry state routine.
 * @param entryAction Pointer to the entry action function.
 * @param fsmEndingAction Pointer to the FSM endign action to be executed upon ending the FSM.
 */
void fsmInitSingleThreaded(
    fsm_t* fsmObject,
    fsmStateRoutine_t entryState,
    fsmAction_t entryAction,
	fsmAction_t fsmEndingAction);

/**
 * @brief Run the FSM.
 * 
 * @param fsmObject Pointer to the FSM object.
 * @return Status of the FSM (fsmStatus_t).
 */
fsmStatus_t fsmRun(
    fsm_t* fsmObject);

/**
 * @brief Transition to the next state in the FSM.
 * 
 * @param fsmObject Pointer to the FSM object.
 * @param nextState Pointer to the next state routine.
 * @param action Pointer to the action function to be executed during transition.
 */
void fsmTransitionState(
    fsm_t* fsmObject,
    fsmStateRoutine_t nextState,
    fsmAction_t action);

/**
 * @brief End the FSM.
 * 
 * @param fsmObject Pointer to the FSM object.
 */
void fsmEndFSM(
    fsm_t* fsmObject);

/**
 * @brief No action function for the FSM.
 */
void fsmNoAction();

#ifdef __cplusplus
}
#endif

#endif // _TINYFSM_H_
