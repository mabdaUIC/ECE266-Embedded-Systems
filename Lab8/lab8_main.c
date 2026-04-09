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
//#include "launchpad.h"  // For PushButton functions
#include "seg7.h"       // For Seg7 functions

/*
 * Global variables
 */


// System state
typedef enum {
    Millimeter,
    Inch
} DisplayMode;

DisplayMode g_mode = Millimeter;
Event trigger_ranger_reading_event;

/*******************************************
 * Task 1: Trigger and read the ranger
 ******************************************/

// Trigger the ranger reading every 0.5 seconds
void TriggerRangerReading(Event *event) {
    RangerTriggerReading();
    while(!RangerDataReady()); // Poll flag
    uint32_t cycles = RangerGetData();

    int buttons = PushButtonRead();

    if (buttons == 1) {
        g_mode = Millimeter;
    } else if (buttons == 2) {
        g_mode = Inch;
    }

    if (g_mode == Millimeter) {
        uint32_t mm = (cycles * 3048) / 896470; // Precise conversion
        Seg7DisplayInt(mm, false);              // No colon
    } else {
        uint32_t total_inches = (cycles * 12) / 89647;
        Seg7DisplayTime(total_inches / 12, total_inches % 12, true); // Colon on
    }

    EventSchedule(event, event->time + 500); // 0.5s interval
}


/*******************************************
 * The main function
 ******************************************/
void main(void)
{
    // Initialize the LaunchPad and peripherals
    LaunchPadInit();
    RangerInit();
    Seg7Init();

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
