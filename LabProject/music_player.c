/*
 * music_player.c – Song playback engine using SysTick for timing.
 * Supports rests (PITCH_REST) and dynamic volume from a global variable.
 */

#include "music_player.h"
#include "music.h"
#include "event.h"
#include <stdbool.h>
#include <stdint.h>

// Global volume percentage (0-100), defined in jukebox_main.c
extern uint8_t current_volume;

static const Song *current_song = NULL;
static uint16_t note_index = 0;
static uint32_t note_start_time = 0;
static uint32_t note_duration = 0;
static PlayerState player_state = PLAYER_STOPPED;

void MusicPlayerInit(void)
{
    // SysTick already running via EventSchedulerInit()
}

void MusicPlayerPlaySong(const Song *song)
{
    if (!song || song->length == 0) return;
    current_song = song;
    note_index = 0;
    player_state = PLAYER_PLAYING;
    note_start_time = EventGetCurrentTime();
    note_duration = song->notes[0].duration;

    uint8_t pitch = song->notes[0].pitch;
    if (pitch == PITCH_REST) {
        MusicTurnOffBuzzer();
        MusicTurnOffLed();
    } else {
        MusicSetBuzzer(pitch, current_volume);   // use global volume
        MusicSetLed(pitch, 2);
    }
}

void MusicPlayerPause(void)
{
    if (player_state == PLAYER_PLAYING) {
        player_state = PLAYER_PAUSED;
        MusicTurnOffBuzzer();
        MusicTurnOffLed();
    }
}

void MusicPlayerResume(void)
{
    if (player_state == PLAYER_PAUSED && current_song) {
        player_state = PLAYER_PLAYING;
        note_start_time = EventGetCurrentTime();
        uint8_t pitch = current_song->notes[note_index].pitch;
        if (pitch == PITCH_REST) {
            MusicTurnOffBuzzer();
            MusicTurnOffLed();
        } else {
            MusicSetBuzzer(pitch, current_volume);
            MusicSetLed(pitch, 2);
        }
    }
}

void MusicPlayerStop(void)
{
    player_state = PLAYER_STOPPED;
    current_song = NULL;
    note_index = 0;
    MusicTurnOffBuzzer();
    MusicTurnOffLed();
}

PlayerState MusicPlayerGetState(void)
{
    return player_state;
}

uint16_t MusicPlayerGetCurrentNoteIndex(void)
{
    return note_index;
}

void MusicPlayerUpdate(void)
{
    if (player_state != PLAYER_PLAYING || !current_song)
        return;

    uint32_t now = EventGetCurrentTime();
    if (now - note_start_time >= note_duration) {
        note_index++;
        if (note_index >= current_song->length) {
            MusicPlayerStop();
            return;
        }
        note_start_time = now;
        note_duration = current_song->notes[note_index].duration;
        uint8_t pitch = current_song->notes[note_index].pitch;
        if (pitch == PITCH_REST) {
            MusicTurnOffBuzzer();
            MusicTurnOffLed();
        } else {
            MusicSetBuzzer(pitch, current_volume);
            MusicSetLed(pitch, 2);
        }
    }
}
