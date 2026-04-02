/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), spring 2026
 *
 * pwmled.c: PWM LED functions
 *
 * Created by Zhao Zhang
 */

#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void PwmBuzzerInit();

/*
 * Set the LED parameters for the three sub-LEDs
 */
void PwmBuzzerSet(uint32_t pulsePeriod, uint32_t pulseWidth);

#endif /* PWDBUZZER_H_ */
