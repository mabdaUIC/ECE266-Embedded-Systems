#include "motion.h"
#include <driverlib/interrupt.h>
#include <inc/hw_ints.h>        // for INT_GPIOC

// Motion sensor state structure
typedef struct {
    Event* callback_event;      // event to schedule on interrupt
    uint32_t raw_input;         // last raw GPIO reading (bitmask)
    bool new_input;             // flag indicating a new reading is available
} MotionState;

// Global state variable (must be non‑static for assembly access)
MotionState motion_sensor = { NULL, 0, false };

// Forward declaration of the ISR
static void MotionISR(void);

<<<<<<< HEAD
=======
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
>>>>>>> 8eae2b7e9bef69a6d8f74fc9f265c13abd5a5074
void MotionInit(void)
{
    // Enable GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while ( !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC) ) {} //wait for clock

    // Configure PC4 as input
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

<<<<<<< HEAD
    // ---- Interrupt configuration ----
    // Register the ISR for Port C
    GPIOIntRegister(GPIO_PORTC_BASE, MotionISR);

    // Trigger on rising edge (motion detected → signal goes high)
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    // Set priority (0 = highest)
    IntPrioritySet(INT_GPIOC, 0);

    // Enable interrupt on pin PC4
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
}

void MotionEventRegister(Event* event)
{
    motion_sensor.callback_event = event;
}

// Motion sensor ISR
static void MotionISR(void)
{
    // Read the current pin value (should be high on rising edge)
    uint32_t pin_value = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Store the reading and set the new input flag
    motion_sensor.raw_input = pin_value;   // non zero if motion detected
    motion_sensor.new_input = true;

    // Schedule the registered callback event (if any)
    if (motion_sensor.callback_event != NULL)
    {
        EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());
    }

    // Clear the interrupt flag – mandatory, otherwise the ISR repeats
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
=======
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
>>>>>>> 8eae2b7e9bef69a6d8f74fc9f265c13abd5a5074
}
