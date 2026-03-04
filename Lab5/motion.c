/*
 * motion.c: C functions for PIR motion sensor
 *
 * ECE 266, spring 2026
 * Lab 4
 */

#include "motion.h"

typedef struct {
    uint32_t isMotionDetected; //1 if motion detected
    uint32_t rawValue; //debug raw value
} MotionState_t;

volatile MotionState_t MotionStatus; //prevent compiler from caching this value

static Event MotionEventTask = 0; //hold the registered event

void Motion_RegisterEvent (Event eventObj) { //registration function
    MotionEventTask = eventObj;
}

// Motion sensor is connected to Grove base J16, Tiva C PC4 (Port C, Pin 4)
void MotionInit(void)
{
    // Enable the GPIO Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while ( !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC) ) {} //wait for clock

    // Configure PC4 as an input pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_RISING_EDGE); //configure interrupt type as rising edge

    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4); //enable the pin interrupt

    IntEnable(INT_GPIOC);
}

void MotionSensorISR() {
    uint32_t pin_value = GPIOinRead(GPIO_PORTC_BASE, GPIO_PIN_4); //read input from motion sensor

    MotionStatus.rawValue = pin_value; //record the input
    MotionStatus.isMotionDetected = 1; //set a flag

    if (MotionEventTask != Null) { //schedule the event if one is registered
        EventSchedule(MotionEventTask, EventGetCurrentTime());
    }

    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4); //clear the interrupt flag
}


