#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"
#include "ras.h"

void DisplayTemperature(float temp_F);
void DisplayRAS(uint16_t angle);

Event ts_trigger_event;
Event ts_data_event;
Event ras_trigger_event;
Event ras_data_event;

//-----------------------------------------------------------------------------
// Trigger functions: only start ADC conversion
//-----------------------------------------------------------------------------
void TriggerTemperatureReading(Event *event)
{
    TsTriggerReading();
    EventSchedule(event, event->time + 4000);   // repeat every 4 s
    uprintf("Temp trigger\n");
}

void TriggerRASReading(Event *event)
{
    RasTriggerReading();
    EventSchedule(event, event->time + 4000);   // repeat every 4 s
    uprintf("RAS trigger\n");
}

//-----------------------------------------------------------------------------
// Data processing functions: called by ISR when fresh data is ready
//-----------------------------------------------------------------------------
void ProcessTemperatureReading(Event *event)
{
    float temp_F = TsDataRead();
    int   temp10 = (int)(temp_F * 10);
    uprintf("Temperature: %u.%u F\r\n", temp10 / 10, temp10 % 10);
    DisplayTemperature(temp_F);
}

void ProcessRASReading(Event *event)
{
    uint16_t angle = RasDataRead();
    uprintf("RAS angle: %u deg\r\n", angle);
    DisplayRAS(angle);
}

//-----------------------------------------------------------------------------
// 7‑segment display functions
//-----------------------------------------------------------------------------
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

void DisplayRAS(uint16_t angle)
{
    Seg7Display seg7;
    uint8_t hundreds = angle / 100;
    uint8_t tens     = (angle / 10) % 10;
    uint8_t ones     = angle % 10;

    seg7.digit[3] = 10;                                      // blank
    seg7.digit[2] = (hundreds == 0) ? 10 : hundreds;        // blank if zero
    seg7.digit[1] = (hundreds == 0 && tens == 0) ? 10 : tens;
    seg7.digit[0] = ones;
    seg7.colon_on = false;
    Seg7Update(&seg7);
    uprintf("RAS digits: %d %d %d %d, colon=%d\r\n",
            seg7.digit[3], seg7.digit[2], seg7.digit[1], seg7.digit[0], seg7.colon_on);
}

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
int main(void)
{
    LaunchPadInit();                     // clocks, SysTick, UART
    uprintf("Starting...\r\n");
    Seg7Init();                          // 7‑segment display
    TsInit();                            // ADC0 seq#0 – internal temp
    RasInit();                           // ADC0 seq#1 – RAS on PE3/AIN0/J5
    uprintf("Init done\r\n");

    // Temperature trigger: first at 100 ms, then every 4 s
    EventInit(&ts_trigger_event, TriggerTemperatureReading);
    EventSchedule(&ts_trigger_event, 100);

    // RAS trigger: first at 2100 ms (2 s offset), then every 4 s
    EventInit(&ras_trigger_event, TriggerRASReading);
    EventSchedule(&ras_trigger_event, 2100);

    // Data events – scheduled by ADC ISRs
    EventInit(&ts_data_event, ProcessTemperatureReading);
    TsEventRegister(&ts_data_event);

    EventInit(&ras_data_event, ProcessRASReading);
    RasEventRegister(&ras_data_event);

    uprintf("Lab 6: Temperature and RAS (ADC)\r\n");

    // Enable global interrupts (should already be on, but just in case)
    IntMasterEnable();

    while (1) {
        __asm("    wfi");
        EventExecute();

        // Optional: you can add a simple heartbeat here without LEDs,
        // e.g., toggle a variable and print a dot every 1000 loops.
        // static uint32_t loop_count = 0;
        // if (++loop_count % 1000 == 0) uprintf(".");
    }
}
