/*
 * motion.c: C functions for PIR motion sensor
 *
 * ECE 266, spring 2026
 * Lab 4
 */

#include "motion.h"

// Motion sensor is connected to Grove base J16, Tiva C PC4 (Port C, Pin 4)
void MotionInit(void)
{
    // Enable the GPIO Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC4 as an input pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
}
