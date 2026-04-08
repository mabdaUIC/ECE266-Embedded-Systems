/*
 * ECE 266 Lab 7 – Music Note Player (PWM)
 * Complete implementation: LED + Buzzer
 *
 * This program allows the user to select a music note (C4 to B4) using SW1
 * and a volume level (low/medium/high) using SW2. The LED changes color and
 * brightness accordingly, and the buzzer plays the corresponding pitch and loudness.
 *
 * The system uses a simple event-driven framework: push buttons generate events,
 * and a periodic timer event toggles the buzzer and LED on/off.
 */

<<<<<<< HEAD
// REVISE AS NEEDED
#include <pwm_buzzer.h>
#include <pwm_led.h>
=======
>>>>>>> 8ce9a1fe9e374a40684e762e4bb625c8f6c024cf
#include "launchpad.h"
#include "music.h"

// Event structures for button checking and sound playback
Event check_push_button_event;
Event play_sound_event;

// System state: current pitch index, volume index, and whether buzzer is playing
typedef struct {
    int pitch;            // 0..NUM_PITCH-1, corresponds to C4..B4
    int volume;           // 0..NUM_VOLUME_LEVEL-1 (low, medium, high)
    bool buzzer_on;       // true if currently playing sound
} SysState;

SysState sys = { 0, 0, false };

/*
 * Task 1: Check the push buttons (SW1 and SW2).
 * Called whenever a button event occurs.
 * SW1 cycles through pitches, SW2 cycles through volume levels.
 */
void CheckPushButton(Event *event)
{
    switch (PushButtonRead()) {
        case 1:                     // SW1 pressed
            sys.pitch = (sys.pitch + 1) % NUM_PITCH;
            break;
        case 2:                     // SW2 pressed
            sys.volume = (sys.volume + 1) % NUM_VOLUME_LEVEL;
            break;
    }
}

/*
 * Task 2: Play sound and control LED.
 * Called periodically by the event scheduler.
 * Toggles the buzzer and LED on/off every 500 ms (on) or 1000 ms (off).
 */
void PlaySound(Event *event)
{
    int delay;
    if (!sys.buzzer_on) {
        // Turn on the buzzer and LED with current pitch and volume settings
        MusicSetLed(sys.pitch, sys.volume);
        MusicSetBuzzer(sys.pitch, sys.volume);
        sys.buzzer_on = true;
        delay = 500;                // play for 500 ms
    } else {
        // Turn off both
        MusicTurnOffLed();
        MusicTurnOffBuzzer();
        sys.buzzer_on = false;
        delay = 1000;               // silent for 1000 ms
    }
    // Schedule the next call after the delay
    EventSchedule(&play_sound_event, event->time + delay);
}

/*
 * Main function: initializes hardware, events, and starts the event loop.
 */
void main(void)
{
    // Initialize LaunchPad hardware (buttons, LEDs, etc.)
    LaunchPadInit();

    // Initialize PWM for the RGB LED (using timers 0 and 1)
    MusicInitLed();

    // Initialize PWM for the buzzer (using a timer and CCP pin)
    MusicInitBuzzer();

    // Initialize event structures
    EventInit(&play_sound_event, PlaySound);
    EventInit(&check_push_button_event, CheckPushButton);

    // Register the button event so that CheckPushButton is called on button presses
    PushButtonEventRegister(&check_push_button_event);

    // Start the sound toggling: first play after 100 ms
    EventSchedule(&play_sound_event, 100);

    // Main loop: wait for interrupts, then execute scheduled callbacks
    while (true) {
        asm(" wfi");                // Wait For Interrupt (low power)
        EventExecute();             // Run any pending events
    }
}
