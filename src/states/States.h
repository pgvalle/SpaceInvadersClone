#ifndef STATES_H
#define STATES_H

void InitStateMachine();
void DestroyStateMachine();

// The state changes are only applied when this function is called.
// May be called only after UpdateCurrentSTate and RenderCurrentState were called.
void UpdateStateChanges();

void UpdateCurrentState();
void RenderCurrentState();


#endif // STATES_H