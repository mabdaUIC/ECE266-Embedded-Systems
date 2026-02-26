/*
 * main.c, starter code
 *
 * ECE 266 Lab 1, Spring 2026
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"

/********************************************************************************
 * Global declarations
 *******************************************************************************/

// Events declaration
Event flash_led_event;
Event push_button_event;

// On-board LED color color setting
// Note: the on-board LED consists of three sub-LEDs, namely red, blue, and green
typedef struct
{
    bool red;                   // Red sub-LED on or off
    bool blue;                  // Blue sub-LED on or off
    bool green;                 // Green sub-LED on or off
} LEDColorSetting;

// A list (array) of LED color settings
#define LED_COLOR_NUM       3
LEDColorSetting led_colors[LED_COLOR_NUM] = {
    { true, false, false },     // Red on, Blue off, Green off
    { false, true, false },     // Red off, Blue on, Green off
    { false, false, true }      // Red off, Blue off, Green on
};

// system state for Lab 1
typedef struct
{
    bool LED_on;                // if the LED is turned on
    int color_choice;           // current color choice
} SysState;

SysState sys =
{
    false,                      // LED turned off,
    0                           // color #0 in led_colors
};

/********************************************************************************
 * Task 1: Flash the LED using the current color setting
 *
 * This is a callback function. Argument time is the current system
 * time in millisecond.
 *******************************************************************************/

void FlashLED(Event *event)
{
    // If on-board LED is turned on, turn it off
    // Otherwise, turn it on with the current color choice
    if (sys.LED_on)
    {
        // Turn off the three sub-LEDs
        LedTurnOnOff(false, false, false);          // see ledTurnOnOff() in launchpad.h
        sys.LED_on = false;
    }
    else
    {
        // Turn on the three sub-LEDs
        LEDColorSetting color = led_colors[sys.color_choice];
        LedTurnOnOff(color.red, color.blue, color.green);
        sys.LED_on = true;
    }

    // Schedule an event to self in 1000 ms
    EventSchedule(event, event->time + 1000);
}

/********************************************************************************
 * Task 2: Check push button.
 *
 * This function is called on push_button interrupt events.
 *******************************************************************************/

void CheckPushButton(Event *event)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int input = PushButtonRead();

    switch (input)
    {
    case 1:
        // Switch to next color setting, with wrap-around
        sys.color_choice = (sys.color_choice + 1) % LED_COLOR_NUM;
        break;

    case 2:
        // Print out a message. See uprintf() in launchpad.h
        uprintf("%s\n\r", "SW2 is pushed");
        break;
    }
}

/********************************************************************************
 * The main function: Print out a message, schedule the first callback events,
 * and then run the callback scheduler.
 *******************************************************************************/

int main(void)
{
    // Initialize the Tiva C LaunchPad, including the clock rate, systick, GPIO,
    // UART0, pushbutton, and event scheduler
    LaunchPadInit();

    // Initialize the event objects
    EventInit(&flash_led_event, FlashLED);
    EventInit(&push_button_event, CheckPushButton);

    // Register ISR (interrupt service routing) scheduled event
    PushButtonEventRegister(&push_button_event);

    // Schedule time event
    EventSchedule(&flash_led_event, 100);

    // Print out a message. See uprintf() in launchpad.h
    uprintf("%s\n\r", "Welcome to ECE 266!");

    // Run the callback scheduler
    // See schdExecute() in launchpad.h
    while (true)
    {
        // Wait for interrupt events (include callback events)
        asm("   wfi");

        // Try executing events
        EventExecute();
    }
}
