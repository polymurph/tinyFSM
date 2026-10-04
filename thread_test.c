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
