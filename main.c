#include <stdio.h>
#include <stdbool.h>
#include "tfsm.h"

typedef struct{
  char pressedCharacter;
}fsm_data_t;

tfsm_state_t state_getUserChar(tfsm_t* fsm);
tfsm_state_t state_outputChar(tfsm_t* fsm);



tfsm_state_t state_getUserChar(tfsm_t* fsm)
{
  // type cast for use
  fsm_data_t* data = (fsm_data_t*)(fsm->contextData);
  fflush(stdout); 
  printf("The test is now in state \"state_getUserChar\"\n");
  printf("Type in any character to move to the next state: ");
  
  fflush(stdout);
  scanf(" %c", &data->pressedCharacter);

  return (tfsm_state_t){state_outputChar};
}

tfsm_state_t state_outputChar(tfsm_t* fsm)
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
    return (tfsm_state_t){tfsm_end};
  }

  return (tfsm_state_t){state_getUserChar};
}


int main(){

  printf("tiny FSM test");

  tfsm_t fsm;
  fsm_data_t fsm_data;
  if(!tfsm_init(&fsm, (tfsm_state_t){state_getUserChar}, (void*) &fsm_data)){
    printf("Failed to initialize FSM!");
    return 1;
  }

  while(tfsm_routine(&fsm));

  printf("Terminated FSM!");

  return 0;
}




