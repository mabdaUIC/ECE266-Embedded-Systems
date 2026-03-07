#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"
#include "pushbutton.h"          // Use the real pushbutton header, not the sample

/********************************************************************************
 * Global declarations
 *******************************************************************************/
typedef struct {
    bool activated;
    bool alerted;
    bool buzzer_on;
} SysState;

static SysState sys = { false, false, false };
static uint32_t LastMotionTime = 0;   // for 3‑second hold

// Event objects
Event buzzer_play_event;
Event check_push_button_event;
Event check_motion_event;

/********************************************************************************
 * Task 1: Buzzer play (callback)
 *******************************************************************************/
void BuzzerPlay(Event *event)
{
    uint32_t delay = 500;

    if (sys.alerted) {
        if (sys.buzzer_on) {
            BuzzerOff();
            sys.buzzer_on = false;
            delay = 2988;
        } else {
            BuzzerOn();
            sys.buzzer_on = true;
            delay = 15;
        }
    } else {
        if (sys.buzzer_on) {
            BuzzerOff();
            sys.buzzer_on = false;
        }
    }
    EventSchedule(event, event->time + delay);
}

/********************************************************************************
 * Task 2: Check motion (callback) – now reads interrupt‑updated state
 *******************************************************************************/
void CheckMotion(Event *event)
{
    int32_t motion = MotionRead();          // returns last recorded motion

    if (motion) {
        LastMotionTime = event->time;
    }

    if (sys.activated) {
        uint32_t timeSinceMotion = event->time - LastMotionTime;
        sys.alerted = (timeSinceMotion < 3000);   // 3‑second hold
    } else {
        sys.alerted = false;
    }

    // Update LED
    if (sys.activated) {
        if (sys.alerted)
            LedTurnOnOff(true, false, false);   // red
        else
            LedTurnOnOff(false, false, true);   // green
    } else {
        LedTurnOnOff(false, false, false);      // off
    }

    EventSchedule(event, event->time + 200);     // check every 200 ms
}

/********************************************************************************
 * Task 3: Check push buttons (called from ISR, not a callback)
 *******************************************************************************/
void CheckPushButton(Event *event)
{
    int code = PushButtonRead();
    switch (code) {
    case 1:                     // SW1
        if (!sys.activated) {
            sys.activated = true;
            // alerted will be set by CheckMotion
        }
        break;
    case 2:                     // SW2
        if (sys.activated) {
            sys.activated = false;
            sys.alerted = false;
            LedTurnOnOff(false, false, false);
        }
        break;
    }
}

/********************************************************************************
 * main()
 *******************************************************************************/
int main(void)
{
    // Initialize hardware
    LaunchPadInit();
    BuzzerInit();
    MotionInit();

    uprintf("%s\n\r", "Lab 5: Motion Detection Alarm (Interrupt based)");

    // Initialize event objects
    EventInit(&buzzer_play_event, BuzzerPlay);
    EventInit(&check_push_button_event, CheckPushButton);
    EventInit(&check_motion_event, CheckMotion);

    // Register callback events with the ISRs
    PushButtonEventRegister(&check_push_button_event);
    MotionEventRegister(&check_motion_event);

    // Schedule first callback events
    EventSchedule(&buzzer_play_event, 100);
    EventSchedule(&check_motion_event, 200);   // keep periodic check

    // Run the callback scheduler
    while (true) {
        asm("   wfi");
        EventExecute();
    }
}
