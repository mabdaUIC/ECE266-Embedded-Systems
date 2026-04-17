/*
 * music_player.c – Song playback using Timer2 (1 ms tick)
 * Debug: Toggles PF0 in ISR and in Update to verify execution.
 */

#include "music_player.h"
#include "music.h"
#include "pwm_led.h"
#include "pwm_buzzer.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"

// Current playback context
static const Song *current_song = NULL;
static uint16_t note_index = 0;
static uint32_t note_start_time = 0;
static uint32_t note_duration = 0;
static PlayerState player_state = PLAYER_STOPPED;

// Tick counter (incremented every 1 ms by Timer2)
static volatile uint32_t g_music_ticks = 0;

// Timer2 interrupt handler
void Timer2IntHandler(void) {
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    g_music_ticks++;

    // Debug: Toggle PF0 (green LED on LaunchPad) to show ISR is firing
    static int toggle = 0;
    toggle = !toggle;
    if (toggle)
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
    else
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
}

void MusicPlayerInit(void) {
    // Enable Timer2 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    // Configure Timer2A as periodic timer
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    // Load value for 1 ms (assumes 50 MHz system clock)
    TimerLoadSet(TIMER2_BASE, TIMER_A, 50000 - 1);
    // Enable timeout interrupt
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER2A);
    // Start timer
    TimerEnable(TIMER2_BASE, TIMER_A);

    // Configure PF0 as output for debug toggles (unlock commit first)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
}

void MusicPlayerPlaySong(const Song *song) {
    if (!song || song->length == 0) return;
    current_song = song;
    note_index = 0;
    player_state = PLAYER_PLAYING;
    note_start_time = g_music_ticks;
    note_duration = song->notes[0].duration;
    uint8_t pitch = song->notes[0].pitch;
    MusicSetBuzzer(pitch, 2);
    MusicSetLed(pitch, 2);
}

void MusicPlayerPause(void) {
    if (player_state == PLAYER_PLAYING) {
        player_state = PLAYER_PAUSED;
        MusicTurnOffBuzzer();
        MusicTurnOffLed();
    }
}

void MusicPlayerResume(void) {
    if (player_state == PLAYER_PAUSED && current_song) {
        player_state = PLAYER_PLAYING;
        note_start_time = g_music_ticks;
        uint8_t pitch = current_song->notes[note_index].pitch;
        MusicSetBuzzer(pitch, 2);
        MusicSetLed(pitch, 2);
    }
}

void MusicPlayerStop(void) {
    player_state = PLAYER_STOPPED;
    current_song = NULL;
    note_index = 0;
    MusicTurnOffBuzzer();
    MusicTurnOffLed();
}

PlayerState MusicPlayerGetState(void) {
    return player_state;
}

uint16_t MusicPlayerGetCurrentNoteIndex(void) {
    return note_index;
}

void MusicPlayerUpdate(void) {
    // Debug: Toggle PF0 inside Update to show it's being called
    static int update_toggle = 0;
    update_toggle = !update_toggle;
    if (update_toggle)
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
    else
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);

    if (player_state != PLAYER_PLAYING || !current_song)
        return;

    uint32_t elapsed = g_music_ticks - note_start_time;
    if (elapsed >= note_duration) {
        note_index++;
        if (note_index >= current_song->length) {
            MusicPlayerStop();
            return;
        }
        note_start_time = g_music_ticks;
        note_duration = current_song->notes[note_index].duration;
        uint8_t pitch = current_song->notes[note_index].pitch;
        MusicSetBuzzer(pitch, 2);
        MusicSetLed(pitch, 2);
    }
}
