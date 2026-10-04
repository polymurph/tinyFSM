# TinyFSM (tfsm)

A minimalist, high-performance, and thread-safe Finite State Machine (FSM) framework written in pure C. It is completely decoupled from any operating system and tailored specifically for embedded microcontrollers, RTOS environments (FreeRTOS, Zephyr), or multi-threaded POSIX applications.

## Key Features
* **Zero Dependencies:** Compiles with any standard C99 environment.
* **Deadlock-Free Design:** Works safely with standard/normal (non-recursive) mutexes.
* **Granular Contexts:** Pass distinct lock bindings and custom `contextData` objects to multiple independent state engines.

---

## The Thread-Safety Architecture (Crucial Note)
To prevent self-transition deadlocks without relying on complex recursive operating system locks, `tfsm` uses a brief critical section approach:

1. `tfsm_routine` locks your custom mutex context briefly.
2. It takes a local copy of the `currentState` function pointer.
3. It immediately drops the lock.
4. It fires the callback function completely unlocked.

**Result:** State handler callbacks are entirely free to invoke `tfsm_transitionState()` inline using a normal, standard mutex. The engine will safely pivot to the next state on the following routine tick.

---

## Basic Usage Example (POSIX Threads)

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "tfsm.h"

// Custom state machine variables
typedef struct {
    pthread_mutex_t mutex;
    int counter;
} app_context_t;

// State prototypes
void State_Idle(tfsm_t* fsm);
void State_Processing(tfsm_t* fsm);

// Mutex wrapper hooks
void my_lock(tfsm_t* fsm) {
    app_context_t* ctx = (app_context_t*)fsm->contextData;
    pthread_mutex_lock(&ctx->mutex);
}

void my_unlock(tfsm_t* fsm) {
    app_context_t* ctx = (app_context_t*)fsm->contextData;
    pthread_mutex_unlock(&ctx->mutex);
}

void State_Idle(tfsm_t* fsm) {
    app_context_t* ctx = (app_context_t*)fsm->contextData;
    printf("FSM Idle. Counter: %d\n", ctx->counter);
    
    if (ctx->counter >= 2) {
        tfsm_transitionState(fsm, State_Processing); // Safely trigger inline!
    }
    ctx->counter++;
}

void State_Processing(tfsm_t* fsm) {
    printf("FSM Processing complete. Exiting...\n");
    tfsm_transitionState(fsm, NULL); // Transitioning to NULL terminates the machine
}

int main(void) {
    tfsm_t my_fsm;
    app_context_t app_ctx = { .mutex = PTHREAD_MUTEX_INITIALIZER, .counter = 0 };

    tfsm_init(&my_fsm, State_Idle, &app_ctx, my_lock, my_unlock);

    while (tfsm_routine(&my_fsm) == FSM_RUNNING) {
        usleep(100000); // 100ms cycle tick
    }

    printf("FSM Finished cleanly.\n");
    return 0;
}
```
