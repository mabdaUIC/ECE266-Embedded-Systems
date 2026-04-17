/*
 * ranger.c – Ultrasonic ranger driver using timer capture
 *
 * This driver uses a wide timer (WTimer2, sub timer B) in two modes:
 *   1. PWM mode to generate a 5 µs start pulse.
 *   2. Input Edge Time mode to capture the rising and falling edges
 *      of the echo pulse. The measured pulse width (in clock cycles)
 *      is directly proportional to the distance.
 *
 * Hardware connections:
 *   Ranger signal pin -> PD1 (GPIO) -> WT2CCP1 (timer capture/compare pin)
 */

#include "ranger.h"
#include <driverlib/timer.h>   // for direct register access

/* --------------------------------------------------------------------
 * Internal state of the ranger driver
 * -------------------------------------------------------------------- */
typedef struct {
    Event *callback_event;          // event to schedule when a new measurement is ready
    enum {
        IDLE,                       // no measurement in progress
        EXPECT_START_EVENT_1,       // waiting for rising edge of start pulse
        EXPECT_START_EVENT_2,       // waiting for falling edge of start pulse
        EXPECT_DATA_EVENT_1,        // waiting for rising edge of echo pulse
        EXPECT_DATA_EVENT_2         // waiting for falling edge of echo pulse
    } state;
    bool new_data_ready;            // flag for polled operation
    uint32_t rising_time;           // timer value at rising edge of echo
    uint32_t falling_time;          // timer value at falling edge of echo
    uint32_t pulse_width;           // difference (falling - rising) in clock cycles
} RangerState;

volatile RangerState ranger;        // state is modified inside ISR

/* Forward declaration of the interrupt handler */
void RangerISR(void);

/* --------------------------------------------------------------------
 * Initialize the timer and GPIO for the ranger.
 * The timer is initially set to Input Edge Time mode with maximum load
 * so that it is ready for capture. It will be reconfigured to PWM mode
 * each time a reading is triggered.
 * -------------------------------------------------------------------- */
void RangerInit(void)
{
    // Clear state
    ranger.callback_event = NULL;
    ranger.new_data_ready = false;
    ranger.state = IDLE;
    ranger.rising_time = 0;
    ranger.falling_time = 0;
    ranger.pulse_width = 0;

    // Enable clock to GPIO Port D (for PD1) and Wide Timer 2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);

    // Configure PD1 as a timer pin and connect it to WT2CCP1
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD1_WT2CCP1);

    // Set up the timer in 32 bit Input Edge Time mode (split pair, timer B only)
    TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME);
    TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    TimerLoadSet(WTIMER2_BASE, TIMER_B, 0xFFFFFFFF);   // free running, no wrap

    // Register ISR and enable capture interrupt
    TimerIntRegister(WTIMER2_BASE, TIMER_B, RangerISR);
    TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);
}

/* --------------------------------------------------------------------
 * Trigger a new distance measurement.
 * This function reconfigures the timer to PWM mode to generate a 5 µs
 * high pulse on the CCP pin, which acts as the start pulse for the ranger.
 * -------------------------------------------------------------------- */
void RangerTriggerReading(void)
{
    ranger.new_data_ready = false;
    ranger.state = EXPECT_START_EVENT_1;

    TimerDisable(WTIMER2_BASE, TIMER_B);

    // Reconfigure timer B as PWM to generate a single 5 µs pulse.
    // Assuming system clock = 50 MHz (20 ns period). 5 µs = 250 cycles.
    // Load = 500, match = 250 gives a 5 µs high time.
    TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER2_BASE, TIMER_B, 500);
    TimerMatchSet(WTIMER2_BASE, TIMER_B, 250);
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);   // clear any stale interrupt

    TimerEnable(WTIMER2_BASE, TIMER_B);   // start PWM – pin goes high, then low after 5 µs
}

/* --------------------------------------------------------------------
 * Return true if a new measurement is available (polling mode).
 * -------------------------------------------------------------------- */
bool RangerDataReady(void)
{
    return ranger.new_data_ready;
}

/* --------------------------------------------------------------------
 * Retrieve the measured pulse width in clock cycles.
 * -------------------------------------------------------------------- */
uint32_t RangerGetData(void)
{
    return ranger.pulse_width;
}

/* --------------------------------------------------------------------
 * Register an event that will be scheduled when a measurement completes.
 * -------------------------------------------------------------------- */
void RangerEventRegister(Event *event)
{
    ranger.callback_event = event;
}

/* --------------------------------------------------------------------
 * Timer capture interrupt service routine.
 * Implements a finite state machine that handles the four edges:
 *   start pulse rising, start pulse falling,
 *   echo pulse rising, echo pulse falling.
 * -------------------------------------------------------------------- */
void RangerISR(void)
{
    uint32_t current_time = TimerValueGet(WTIMER2_BASE, TIMER_B);

    switch (ranger.state)
    {
    case EXPECT_START_EVENT_1:
        // Rising edge of start pulse – just move to next state
        ranger.state = EXPECT_START_EVENT_2;
        break;

    case EXPECT_START_EVENT_2:
        // Falling edge of start pulse – now reconfigure timer for capture
        TimerDisable(WTIMER2_BASE, TIMER_B);

        // Switch to Input Edge Time mode, count UP, capture both edges
        TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME);
        TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
        TimerLoadSet(WTIMER2_BASE, TIMER_B, 0xFFFFFFFF);   // large load prevents wrap

        // Force the timer to count UP (default after reset is DOWN)
        HWREG(WTIMER2_BASE + TIMER_O_TBMR) |= TIMER_TBMR_TBCDIR;

        TimerEnable(WTIMER2_BASE, TIMER_B);
        ranger.state = EXPECT_DATA_EVENT_1;
        break;

    case EXPECT_DATA_EVENT_1:
        // Rising edge of echo pulse – record the current timer value
        ranger.rising_time = current_time;
        ranger.state = EXPECT_DATA_EVENT_2;
        break;

    case EXPECT_DATA_EVENT_2:
        // Falling edge of echo pulse – record value and compute pulse width
        ranger.falling_time = current_time;
        // Because the timer is counting up, falling_time >= rising_time
        ranger.pulse_width = ranger.falling_time - ranger.rising_time;
        ranger.new_data_ready = true;

        // If a callback event was provided, schedule it
        if (ranger.callback_event != NULL)
            EventSchedule(ranger.callback_event, 0);

        ranger.state = IDLE;
        break;

    default:
        // Unexpected state – ignore
        break;
    }

    // Clear the capture interrupt flag
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
}
