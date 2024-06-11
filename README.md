# tinyFSM

lightweight finite state machine (FSM) framework written in C.

This framework allows fast and easy implementation and has no dependencies to any libraryes.

## How to implement tinyFSM

### Implementation <a name="p_1"></a>

First create a fsm object.
```c
  fsm_t fsm;
```
It then needs to be initialized.
```c
  fsmInit(&fsm, entryState, entryAction);
```
Once the fsm is initialized the fsm can be executed in a loop e.g. 
```c
  while(fsmRun(&fsm)!=FSM_RUNNING);
```
The constant checking if the return of `FSM_RUNNING` is done in order to stop looping if the FSM ended or a fault occured.

### Creating states  <a name="p_2"></a>

The states must be of the form shown here.
```c
void state_example(void)
{
  // your code here
}
```
It must have an ```c void``` as argument and return.

> **_NOTE:_**  no infinite loops are allowed inside this function, as this will inhibit the functionality of the FSM.

### Creating actions  <a name="p_3"></a>

The action must be of the form shown here.
```c
void action_example(void)
{
  // your code here
}
```
It must have an ```c void``` as argument and return.

> **_NOTE:_**  no infinite loops are allowed inside this function, as this will inhibit the functionality of the FSM.

### Transitioning states  <a name="p_4"></a>

The followin example shows how a state can be transitioned.

```c
void state_example(void)
{
  // your code here
  
  if(your condition){
    fsmTransitionState(&fsmObject, next_state, state_transition_action);
    return;
  }
}
```

## TODOs

TODO: add nee feature to save ending action for the fsm
TODO: add mutex for thread safety

Initial Author Edwin Koch
This work is licensed under MIT. See the LICENSE file in the project root for more information.
