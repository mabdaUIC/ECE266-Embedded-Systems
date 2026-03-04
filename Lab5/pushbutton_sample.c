/* pushbutton.c
 *
 * Push button functions for the LaunchPad, simplified for 
 * Lab 5 as a sample of interrupt programming
 *
 * For reading only -- do NOT include this file in CCS project.
 *
 * Created by Zhao Zhang
 *
 * Pushbutton pin map: SW1 => PF4, SW2 => PF0
 *
 */

#include "launchpad.h"
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>

/*
 * Global declarations
 */

// Push button input state
typedef struct {
    Event *callback_event;          // pointer to Event object
    uint32_t raw_input;             // the raw input from GPIO port pins
    bool new_input;                 // flag for new input data
} PushButtonState;

static PushButtonState push_button;

// pre-declare the ISR function
static void PushButtonISR();

/*
 * Initialize the push button with interrupt enabled
 */
void PushButtonInit()
{
    // Initialize push button state
    push_button.callback_event = NULL;
    push_button.debouncing_delay = 250;
    push_button.new_input = false;

    // Enable PF and configure PF0 and PF4 to output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    // The following is for the push buttons only -- NOT needed for 
    // motion sensor.
    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    // Set the pad(s) for open-drain operation with a weak pull-up.
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
            GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
    // register the interrupt handler, set interrupt on falling edge (push
    // button is active low), set interrupt priority level to 0, enable
    // the interrupt on SW1 and SW2 input pins.
    //
    // Note: push button input is active low. Motion sensor input is not.
    GPIOIntRegister(GPIO_PORTF_BASE, PushButtonISR);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, 
                   GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF, 0);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
}

void PushButtonEventRegister(Event* callback_event)
{
    assert(EventInitialized(callback_event));
    push_button.callback_event = callback_event;
}

void PushButtonSetDebouncingDelay(int debouncing_delay)
{
    push_button.debouncing_delay = debouncing_delay;
}

/*
 * Check and return push button input
 */
int PushButtonRead()
{
    // If there is no new input, return 0
    if (!push_button.new_input)
        return 0;

    // Reset new input flag
    push_button.new_input = false;

    // Convert raw input to input code and return it
    if (push_button.raw_input & GPIO_PIN_4) {
        return 1;       // for SW1
    }
    else if (push_button.raw_input & GPIO_PIN_0) {
        return 2;       // for SW2
    }
    else
        return 0;
}

/*
 * Push button ISR
 */
static void PushButtonISR()
{
    static uint32_t last_event_time = 0;      // remember last pushing time

    // Read Port F. SW1 and SW2 are active low, so invert the reading
    uint32_t pin_value = ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);

    // Record the push button raw input, no de-bouncing
    push_button.raw_input = pin_value;
    push_button.new_input = true;

    // Schedule callback event
    if (push_button.callback_event != NULL)
        EventSchedule(push_button.callback_event, EventGetCurrentTime());

    // IMPORTANT: Clear interrupt flag
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
}

