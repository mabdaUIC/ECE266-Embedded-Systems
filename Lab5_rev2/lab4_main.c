/*
 * Lab 4 starter code
 * ECE 266, spring 2026
 *
 * lab4_main.c: Implement the tasks for a motion detection alarm.
 *
 * Created by Zhao Zhang
 * Revised by Group F3
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"    // New header for motion sensor

/********************************************************************************
 * Global declarations
 *******************************************************************************/

// The data structure for the system state
typedef struct {
    // Activated state: if the system is activated or not
    bool activated;

    // Alert state: If the alert is on or off
    bool alerted;

    // Sound state: If the buzzer is turned on to make a sound or not
    // Note: When the system is activated and alerted, the buzzer is turned
    // on and off periodically
    bool buzzer_on;
} SysState;

// The system state
static SysState sys =
{
    false /* not activated */,
    false /* not alerted */,
    false /* buzzer not turned on */
};


// Optional: for 3-second hold feature
static uint32_t LastMotionTime = 0; // time when motion was last seen

// The events
Event buzzer_play_event;
Event check_push_button_event;
Event check_motion_event;           // New event for motion polling

/********************************************************************************
 * Task 1: If the system is activated and alerted, make the buzzer play
 * on/off sound. The buzzer will be repeatedly turned on and off.
 *
 * You may adjust the on and off times to change the sound pattern and intensity.
 *
 * Note: This is a callback function.
 *******************************************************************************/

void BuzzerPlay(Event *event)                   // the scheduled time
{
    uint32_t delay = 500;                       // default checking interval

    if (sys.alerted)
    {
        // Turn buzzer on and off
        if (sys.buzzer_on)
        {
            // The buzzer is on: turn it off
            BuzzerOff();
            sys.buzzer_on = false;
            delay = 2988;                       // off for 2988 ms
        }
        else
        {
            // The buzzer is off: turn it on
            BuzzerOn();
            sys.buzzer_on = true;
            delay = 15;                         // on for 12 ms
        }
    }
    else
    {
        // Make sure the buzzer is turned off
        if (sys.buzzer_on)
        {
            BuzzerOff();
            sys.buzzer_on = false;
        }
    }

    // schedule callback to buzzer play
    EventSchedule(event, event->time + delay);
}

/********************************************************************************
 * Task 2: YOUR CODE, check the motion sensor
 *
 * Check the motion sensor input. If motion is detected, put the system
 * in the alerted state. Otherwise, put it in the non-alerted state.
 *
 * This is a callback function. In the next lab, you will re-write this function
 * to make it interrupt based.
 *******************************************************************************/

void CheckMotion(Event *event)
{
    // Read motion sensor (assembly function returns non-zero if motion detected
    int32_t motion = MotionRead();

    // Optional: For 3-second hold, record the time when motion is seen
    // Requires a global variable and using event->time (current time)
    if (motion)
    {
        LastMotionTime = event->time;       // update last motion time
    }

    if (sys.activated)
    {
        // Decide alert state
        //Basic version: alert = motion detected now
        // Optional 3-second hold: alert = (event->time - lastMotionTime) < 3000 ms
        uint32_t timeSinceMotion = event->time - LastMotionTime;
        sys.alerted = (timeSinceMotion < 3000); // hold for 3 seconds
        // (Optional) Print status for debugging
        // uprintf("Motion: %d, alerted: %d\n\r", motion, sys.alerted);
    }
    else
    {
        sys.alerted = false;
    }

    // Update LED color based on alert state (and activation)
    if (sys.activated)
    {
        if (sys.alerted)
            LedTurnOnOff(true, false, false); // red
        else
            LedTurnOnOff(false, false, true); // green
    }
    else
    {
        LedTurnOnOff(false, false, false); // off
    }

    // Schedule next check (e.g., every 200 ms)
    EventSchedule(event, event->time + 200);
}

/********************************************************************************
 * Task 3: Check push buttons.
 *
 * In the starter code: Pushing SW1 activates the system and put it in the
 * alerted state. Pushing SW2 deactivates the system.
 *
 * IN YOUR SOLUTION CODE: Pushing SW1 activates the system. Pushing SW2
 * deactivates the system. If the system is activated, the motion sensor
 * input decides if the system is put in the alerted state or not.
 *
 * This function is called upon interrupts. It is NOT a callback function.
 *
 * YOU NEED TO REVISE THIS FUNCTION
 *
 *******************************************************************************/

void CheckPushButton(Event *event)
{
    int code = PushButtonRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1 is pushed
        if (!sys.activated)
        {
            // Activate the system
            sys.activated = true;

            // Not Needed since sys.alerted will be set by CheckMotion
            // sys.alerted = true;
            // LedTurnOnOff(true /* red */, false /* blue */, false /* green */);

        }
        break;

    case 2:                     // SW2 is pushed
        if (sys.activated)
        {
            // De-activate the system and turn off LED
            sys.activated = false;
            sys.alerted = false;
            LedTurnOnOff(false /* red */, false /* blue */, false /* green */);
        }
        break;
    }
}


/********************************************************************************
 * The main function: Print out a message, schedule the first callback event(s),
 * and then run the callback scheduler.
 *******************************************************************************/

int main(void)
{
    // Initialize the Tiva C LaunchPad and 7-seg
    LaunchPadInit();
    BuzzerInit();
    MotionInit();       // Initialized motion sensor

    uprintf("%s\n\r", "Lab 4: Motion ");

    // Initialize the event objects
    EventInit(&buzzer_play_event, BuzzerPlay);
    EventInit(&check_push_button_event, CheckPushButton);
    EventInit(&check_motion_event, CheckMotion); // New event

    // Register ISR event
    PushButtonEventRegister(&check_push_button_event);

    // Schedule initial call backs
    EventSchedule(&buzzer_play_event, 100);
    EventSchedule(&check_motion_event, 200);    // Start polling motion

    // Run the callback scheduler
    while (true)
    {
        // Wait for interrupt events (include callback events)
        asm("   wfi");

        // Run callback events
        EventExecute();
    }
}
