# tinyFSM

lightweight finite state machine (FSM) framework written in C.

This framework allows fast and easy implementation.

## How to implement tinyFSM

### Implementation

First create a fsm object `fsm_t fsm;`.
It then needs to be initialized `fsmInit(&fsm, entryState, entryAction)`.
Once the fsm is initialized the fsm can be executed in a loop e.g. 
`while(fsmRun(&fsm)!=FSM_RUNNING);`.
The constant checking if the return of `FSM_RUNNING` is done in order to stop looping if the FSM ended or a fault occured.

### Creating states


TODO: add nee feature to save ending action for the fsm

TODO: add mutex for thread safety
