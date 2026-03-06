#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "launchpad.h"          // for Event type

// Initialize the PIR motion sensor (connected to PC4)
void MotionInit(void);

// Register an event object to be scheduled when motion is detected
void MotionEventRegister(Event* event);

// Read the motion sensor input. Returns non‑zero if motion detected, zero otherwise.
// This function returns the last recorded input from the ISR and clears the new‑input flag.
int32_t MotionRead(void);

#endif /* MOTION_H_ */
