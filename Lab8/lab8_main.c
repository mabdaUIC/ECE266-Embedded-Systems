/*
 * Lab 8: Distance Measurement
 *
 * lab8_main.c: Starter code for ECE 266 Lab 8, spring 2026
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"

/*
 * Global variables
 */

// System state
typedef struct {
    enum {Millimeter, Inch} display_mode;       // display inch or millimeter
} SysState;

// The events
Event trigger_ranger_reading_event;

SysState sys = {Millimeter};

/*******************************************
 * Task 1: Trigger and read the ranger
 ******************************************/

// Trigger the ranger reading every 0.5 seconds
void TriggerRangerReading(Event *event)
{
    RangerTriggerReading();

    EventSchedule(event, event->time + 500);
}


/*******************************************
 * The main function
 ******************************************/
void main(void)
{
    // Initialize the LaunchPad and peripherals
    LaunchPadInit();
    RangerInit();

    // Initialize the events
    EventInit(&trigger_ranger_reading_event, TriggerRangerReading);

    // Schedule time event
    EventSchedule(&trigger_ranger_reading_event, 100);

    // Loop forever
    while (true)
    {
        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
