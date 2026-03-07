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

void MotionInit(void)
{
    // Enable GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)); // wait for peripheral

    // Configure PC4 as input
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

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

    // Store the reading and set the new‑input flag
    motion_sensor.raw_input = pin_value;   // non‑zero if motion detected
    motion_sensor.new_input = true;

    // Schedule the registered callback event (if any)
    if (motion_sensor.callback_event != NULL)
    {
        EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());
    }

    // Clear the interrupt flag – mandatory, otherwise the ISR repeats
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
}
