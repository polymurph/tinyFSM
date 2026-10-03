#include <stdio.h>
#include <stdbool.h>
#include "tfsm.h"

typedef struct{
  char pressedCharacter;
  tfsm_t fsm;
}fsm_data_t;

void state_getUserChar(tfsm_t* fsm);
void state_outputChar(tfsm_t* fsm);

void state_getUserChar(tfsm_t* fsm)
{
    fsm_data_t* data = (fsm_data_t*)(fsm->contextData);
    // type cast for use
    fflush(stdout); 
    printf("The test is now in state \"state_getUserChar\"\n");
    printf("Type in any character to move to the next state: ");

    fflush(stdout);
    scanf(" %c", &data->pressedCharacter);
    tfsm_transitionState(fsm, state_outputChar);  
}

void state_outputChar(tfsm_t* fsm)
{
  char userChoice = 'a';

  fsm_data_t* data = (fsm_data_t*)(fsm->contextData); 

  printf("The test is now in state \"state_outputChar\"\n");
  printf("You pressed the character %c!\n", data->pressedCharacter);

  printf("To stop press [x] and to go on any other key!\n");

  fflush(stdout);
  scanf(" %c", &userChoice);

  if(userChoice == 'x')
  {
    printf("About to terminate the FSN...");
    tfsm_transitionState(fsm, NULL);
    return;
  }

  tfsm_transitionState(fsm, state_getUserChar);
}


int main(){

  printf("tiny FSM test");

  fsm_data_t fsm_data;
  if(!tfsm_init(&fsm_data.fsm, state_getUserChar, (void*){&fsm_data}))
  {
    printf("Failed to initialize FSM!");
    return 1;
  }

  while(tfsm_routine(&fsm_data.fsm));

  printf("Terminated FSM!");

  return 0;
}




