/*
 * Starter code for ECE 266 Lab 6, main.c, fall 2026
 *
 * Lab 6: Temperature reading and knob input (ADC)
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"

// The events
Event ts_trigger_event;
Event ts_data_event;
Event ras_trigger_event;
Event ras_data_event;

/*
 * Trigger the temperature reading periodically
 */
void TriggerTemperatureReading(Event *event)
{
    // Trigger ADC sampling for the temperature sensor
    TsTriggerReading();

    // Schedule the next callback in 3000 milliseconds
    EventSchedule(event, event->time + 3000);
}

/*
 * Process temperature reading data
 */
void ProcessTemperatureReading(Event *event)
{
    float temp_F = TsDataRead();
    int temp10 = temp_F * 10;

    // Print on terminal
    uprintf("Temperature Reading: %u.%u F\r\n", temp10 / 10, temp10 % 10);
}

/*
 * The main function: Initialize the system and run event scheduler loop
 */
int main(void)
{
    // Initialize all hardware components being used
    LaunchPadInit();
    TsInit();

    // Initialize and schedule timing events
    EventInit(&ts_trigger_event, TriggerTemperatureReading);
    EventSchedule(&ts_trigger_event, 100);

    // Initialize and register ISR events
    EventInit(&ts_data_event, ProcessTemperatureReading);
    TsEventRegister(&ts_data_event);

    uprintf("%s\n\r", "Lab 6: Temperature reading and knob input (ADC)");

    // Loop forever
    while (true)
    {
        // Wait for interrupt to happen
        __asm("    wfi");

        // Call the callback scheduler
        EventExecute();
    }
}
