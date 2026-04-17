/*
 * Lab 8 – Distance Measurement with Two Mode Display
 *
 * Main program that:
 *   - Triggers the ultrasonic ranger every 500 ms.
 *   - When a measurement completes, converts the pulse width to distance
 *     in either millimeters or feet+inches.
 *   - Displays the distance on the 4 digit 7 segment display.
 *   - Pushbuttons SW1 (PF4) and SW2 (PF0) change the display mode.
 *   - For Part 2, prints the raw pulse width and calculated distance
 *     to the CCS terminal (UART).
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"
#include <driverlib/sysctl.h>   // for SysCtlClockGet()
#include <driverlib/gpio.h>     // for GPIOPinRead()

/* --------------------------------------------------------------------
 * Global state
 * -------------------------------------------------------------------- */
typedef enum { Millimeter, Inch } DisplayMode;
DisplayMode sys_mode = Millimeter;   // start in millimeter mode

/* Events */
Event trigger_event;                 // triggers a new reading every 500 ms
Event process_event;                // called when a measurement is ready

/* Conversion factors – computed at runtime based on actual system clock */
static uint32_t clock_hz = 0;
static float mm_per_cycle = 0.0f;
static float inch_per_cycle = 0.0f;

/* --------------------------------------------------------------------
 * Event callback: Trigger a new ranger reading and reschedule itself.
 * -------------------------------------------------------------------- */
void TriggerRangerReading(Event *event)
{
    RangerTriggerReading();
    EventSchedule(event, event->time + 500);   // every 500 ms
}

/* --------------------------------------------------------------------
 * Event callback: Process the completed measurement.
 * Converts pulse width to distance and updates the 7 segment display.
 * -------------------------------------------------------------------- */
void ProcessMeasurement(Event *event)
{
    if (!RangerDataReady())
        return;

    uint32_t cycles = RangerGetData();

    // Part 2: print raw cycles to CCS terminal
    printf("Cycles = %u\n", cycles);

    Seg7Display disp;
    disp.colon_on = false;

    if (sys_mode == Millimeter)
    {
        // Convert cycles to millimeters
        uint32_t mm = (uint32_t)(cycles * mm_per_cycle);
        if (mm > 9999) mm = 9999;          // display limit

        // digit[0] is rightmost (units), digit[3] is leftmost
        disp.digit[0] = mm % 10;
        disp.digit[1] = (mm / 10) % 10;
        disp.digit[2] = (mm / 100) % 10;
        disp.digit[3] = (mm / 1000) % 10;
        // colon off
    }
    else   // Inch mode
    {
        // Convert cycles to inches, then to feet + inches
        float inches = cycles * inch_per_cycle;
        uint32_t total_inches = (uint32_t)(inches + 0.5f);   // round to nearest inch
        uint32_t feet = total_inches / 12;
        uint32_t inch = total_inches % 12;

        disp.colon_on = true;
        // Display as "xx:yy" where xx = feet, yy = inches (no leading zeros)
        disp.digit[0] = inch % 10;
        disp.digit[1] = inch / 10;
        disp.digit[2] = feet % 10;
        disp.digit[3] = feet / 10;
    }

    Seg7Update(&disp);
}

/* --------------------------------------------------------------------
 * Poll pushbuttons SW1 and SW2 to change display mode.
 * Buttons are active low, so we detect falling edges.
 * -------------------------------------------------------------------- */
void CheckButtons(void)
{
    static uint8_t prev_sw1 = 0, prev_sw2 = 0;

    // Read buttons (0 = pressed, 1 = released)
    uint8_t sw1 = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) ? 0 : 1;
    uint8_t sw2 = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) ? 0 : 1;

    // Falling edge of SW1 -> millimeter mode
    if (sw1 == 0 && prev_sw1 == 1)
        sys_mode = Millimeter;

    // Falling edge of SW2 -> inch mode
    if (sw2 == 0 && prev_sw2 == 1)
        sys_mode = Inch;

    prev_sw1 = sw1;
    prev_sw2 = sw2;
}

/* --------------------------------------------------------------------
 * Main entry point
 * -------------------------------------------------------------------- */
int main(void)
{
    LaunchPadInit();            // initialises board, clocks, UART, etc.
    RangerInit();               // initialises timer and GPIO for ranger
    Seg7Init();                 // initialises 7 segment display (I2C like bit bang)

    // Obtain actual system clock frequency (e.g., 80 MHz)
    clock_hz = SysCtlClockGet();

    // Pre compute conversion factors based on actual clock and sound speed.
    // Derivation:
    //   distance (mm) = (cycles * 340 m/s * 1000 mm/m) / (2 * clock_hz)
    //   = cycles * (340000) / (2 * clock_hz) = cycles * 170000 / clock_hz
    mm_per_cycle = 170000.0f / clock_hz;
    //   inches = mm / 25.4
    inch_per_cycle = mm_per_cycle / 25.4f;

    printf("System clock = %u Hz\n", clock_hz);
    printf("mm per cycle = %f\n", mm_per_cycle);

    // Set up events
    EventInit(&trigger_event, TriggerRangerReading);
    EventInit(&process_event, ProcessMeasurement);
    RangerEventRegister(&process_event);   // ranger will schedule process_event

    // Start the first reading after 100 ms
    EventSchedule(&trigger_event, 100);

    // Main loop – wait for interrupts, execute events, check buttons
    while (1)
    {
        asm(" wfi");                // wait for interrupt (saves power)
        EventExecute();             // run any scheduled callbacks
        CheckButtons();             // poll pushbuttons (they are not interrupt driven)
    }
}
