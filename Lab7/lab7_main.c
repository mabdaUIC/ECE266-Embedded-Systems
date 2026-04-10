/*
 * ECE 266 Lab 7 – Music Note Player (PWM)
 * Complete implementation: LED + Buzzer
 */

#include "launchpad.h"
#include "music.h"

// Event structures
Event check_push_button_event;
Event play_sound_event;

// System state
typedef struct {
    int pitch;            // 0..NUM_PITCH-1 (C4..B4)
    int volume;           // 0..NUM_VOLUME_LEVEL-1
    bool buzzer_on;       // true if currently playing
} SysState;

SysState sys = { 0, 0, false };

// Task 1: Check push buttons (SW1 = pitch, SW2 = volume)
void CheckPushButton(Event *event)
{
    switch (PushButtonRead()) {
        case 1:
            sys.pitch = (sys.pitch + 1) % NUM_PITCH;
            break;
        case 2:
            sys.volume = (sys.volume + 1) % NUM_VOLUME_LEVEL;
            break;
    }
}

// Task 2: Play sound and update LED (toggles every 500/1000 ms)
void PlaySound(Event *event)
{
    int delay;
    if (!sys.buzzer_on) {
        MusicSetLed(sys.pitch, sys.volume);
        MusicSetBuzzer(sys.pitch, sys.volume);
        sys.buzzer_on = true;
        delay = 500;    // play for 500 ms
    } else {
        MusicTurnOffLed();
        MusicTurnOffBuzzer();
        sys.buzzer_on = false;
        delay = 1000;   // silent for 1000 ms
    }
    EventSchedule(&play_sound_event, event->time + delay);
}

void main(void)
{
    LaunchPadInit();
    MusicInitLed();
    MusicInitBuzzer();

    EventInit(&play_sound_event, PlaySound);
    EventInit(&check_push_button_event, CheckPushButton);
    PushButtonEventRegister(&check_push_button_event);
    EventSchedule(&play_sound_event, 100);

    while (1) {
        asm(" wfi");
        EventExecute();
    }
}
