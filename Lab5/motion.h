/*
 * motion.h: Header file for PIR motion sensor functions
 *
 * ECE 266, spring 2026
 * Lab 4
 */

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

// Initialize the PIR motion sensor (connected to PC4)
void MotionInit(void);


void MotionEventRegister(Event* callback_event); //register an event object with the motion sensor

// Read the motion sensor input.
// Returns non-zero if motion is detected, zero otherwise.
int32_t MotionRead(void);

#endif /* MOTION_H_ */
