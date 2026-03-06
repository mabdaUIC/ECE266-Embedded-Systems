#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"
#include "ras.h"

// Forward declarations — required because calls appear before definitions
void DisplayTemperature(float temp_F);
void DisplayRAS(uint16_t angle);

// Events
Event ts_trigger_event;
Event ts_data_event;
Event ras_trigger_event;
Event ras_data_event;

void TriggerTemperatureReading(Event *event)
{
    TsTriggerReading();
    EventSchedule(event, event->time + 4000);   // repeat every 4 s
    uprintf("Temp event\n");
        float temp_F = TsDataRead();
}

void TriggerRASReading(Event *event)
{
    RasTriggerReading();
    EventSchedule(event, event->time + 4000);   // repeat every 4 s
    uprintf("RAS event\n");
        uint16_t angle = RasDataRead();
}

void ProcessTemperatureReading(Event *event)
{
    float temp_F = TsDataRead();
    int   temp10 = (int)(temp_F * 10);
    uprintf("Temperature Sensor Reading: %u.%u\r\n", temp10 / 10, temp10 % 10);
    DisplayTemperature(temp_F);
}

void ProcessRASReading(Event *event)
{
    uint16_t angle = RasDataRead();
    uprintf("RAS Reading: %u\r\n", angle);
    DisplayRAS(angle);
}

/*
 * Temperature on leftmost 3 digits, rightmost digit blank, colon ON.
 * Example 75.8 F:  [7][5]:[8][ ]
 */

void DisplayTemperature(float temp_F)
{
    Seg7Display seg7;
    int temp10   = (int)(temp_F * 10);
    int integer  = temp10 / 10;
    int fraction = temp10 % 10;

    seg7.digit[3] = integer / 10;   // tens
    seg7.digit[2] = integer % 10;   // ones
    seg7.digit[1] = fraction;       // first decimal
    seg7.digit[0] = 10;             // blank
    seg7.colon_on = true;
    Seg7Update(&seg7);
    uprintf("Temp digits: %d %d %d %d, colon=%d\r\n",
            seg7.digit[3], seg7.digit[2], seg7.digit[1], seg7.digit[0], seg7.colon_on);
}

/*
 * Angle on rightmost 3 digits, leftmost digit blank, colon OFF, no leading zeros.
 * Example 127 deg: [ ][1][2][7]
 * Example  91 deg: [ ][ ][9][1]
 */
void DisplayRAS(uint16_t angle)
{
    Seg7Display seg7;
    uint8_t hundreds = angle / 100;
    uint8_t tens     = (angle / 10) % 10;
    uint8_t ones     = angle % 10;

    seg7.digit[3] = 10;
    seg7.digit[2] = (hundreds == 0) ? 10 : hundreds;
    seg7.digit[1] = (hundreds == 0 && tens == 0) ? 10 : tens;
    seg7.digit[0] = ones;
    seg7.colon_on = false;
    Seg7Update(&seg7);
    uprintf("Temp digits: %d %d %d %d, colon=%d\r\n",
            seg7.digit[3], seg7.digit[2], seg7.digit[1], seg7.digit[0], seg7.colon_on);
}

int main(void)
{
    LaunchPadInit();   // sets up clocks + SysTick event timer
    uprintf("Starting...\r\n");
    Seg7Init();
    TsInit();          // ADC0 seq#0 → internal temperature sensor
    RasInit();         // ADC0 seq#1 → RAS on PE3/AIN0/J5
    uprintf("Init done\r\n");

    // Temperature: first at 100 ms, then every 4 s
    EventInit(&ts_trigger_event, TriggerTemperatureReading);
    EventSchedule(&ts_trigger_event, 100);

    // RAS: first at 2100 ms (2 s offset), then every 4 s → alternates every 2 s
    EventInit(&ras_trigger_event, TriggerRASReading);
    EventSchedule(&ras_trigger_event, 2100);

    // Data callbacks — scheduled by the ADC ISRs
    EventInit(&ts_data_event, ProcessTemperatureReading);
    TsEventRegister(&ts_data_event);

    EventInit(&ras_data_event, ProcessRASReading);
    RasEventRegister(&ras_data_event);

    uprintf("Lab 6: Temperature and RAS (ADC)\r\n");

    while (true) {
        __asm("    wfi");
        EventExecute();
        uprintf(".");
    }
}

