#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include <stdint.h>
#include <stdbool.h>
#include "launchpad.h"

// Initialize pushbutton with interrupt
void PushButtonInit(void);

// Register an event to be scheduled on button press
void PushButtonEventRegister(Event* event);

// Read the button state (returns 1 for SW1, 2 for SW2, 0 if none)
int PushButtonRead(void);

// Set debouncing delay (optional)
void PushButtonSetDebouncingDelay(int delay);

#endif
