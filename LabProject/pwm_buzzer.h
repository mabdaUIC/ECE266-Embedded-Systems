/*
 * pwm_buzzer.h – Low level PWM control for the buzzer
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

// Initialize the buzzer PWM hardware (timer and GPIO)
void PwmBuzzerInit(void);

// Set the PWM period and pulse width (in clock cycles)
void PwmBuzzerSet(uint32_t pulsePeriod, uint32_t pulseWidth);

#endif /* PWM_BUZZER_H_ */
