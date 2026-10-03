#include <stdio.h>
#include <stdbool.h>
#include "tfsm.h"

typedef enum{
    STATE_GET_USER_CHAR,
    STATE_OUTPUT_CHAR,
    STATE_TERMINATED
} dummyApp_state_t;

typedef struct{
    char pressedCharacter;
    tfsm_t fsm;
    dummyApp_state_t state;
}dummyApp_t;

void state_getUserChar(tfsm_t* fsm);
void state_outputChar(tfsm_t* fsm);

void state_getUserChar(tfsm_t* fsm)
{
    dummyApp_t* dummyApp = (dummyApp_t*)(fsm->contextData);
    // type cast for use
    fflush(stdout); 
    printf("The test is now in state \"state_getUserChar\"\n");
    printf("Type in any character to move to the next state: ");

    fflush(stdout);
    scanf(" %c", &dummyApp->pressedCharacter);
    dummyApp->state = STATE_OUTPUT_CHAR;
    tfsm_transitionState(fsm, state_outputChar);  
}

void state_outputChar(tfsm_t* fsm)
{
    char userChoice = 'a';

    dummyApp_t* dummyApp = (dummyApp_t*)(fsm->contextData); 

    printf("The test is now in state \"state_outputChar\"\n");
    printf("You pressed the character %c!\n", dummyApp->pressedCharacter);

    printf("To stop press [x] and to go on any other key!\n");

    fflush(stdout);
    scanf(" %c", &userChoice);

    if(userChoice == 'x')
    {
        printf("About to terminate the FSN...");
        dummyApp->state = STATE_TERMINATED;
        tfsm_transitionState(fsm, NULL);
        return;
    }
    dummyApp->state = STATE_GET_USER_CHAR;
    tfsm_transitionState(fsm, state_getUserChar);
}

bool dummyApp_init(dummyApp_t* dummyApp)
{
    // initialize dummyApp state
    dummyApp->state = STATE_GET_USER_CHAR;
    // initialize dummyApp FSM
    return tfsm_init(&dummyApp->fsm,state_getUserChar, dummyApp);
}

bool dummyApp_routine(dummyApp_t* dummyApp)
{
    return tfsm_routine(&dummyApp->fsm);
}

int main(){

  printf("dummyApp test of tiny FSM");

  dummyApp_t dummyApp;
  if(!dummyApp_init(&dummyApp))
  {
    printf("Failed to initialize dummyApp");
    return 1;
  }

  while(dummyApp_routine(&dummyApp));

  printf("Terminated dummyApp!");

  return 0;
}




