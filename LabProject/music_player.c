/*
 * music_player.c – Song playback engine using SysTick for timing.
 * Supports rests (PITCH_REST) and uses the event system's time.
 */

#include "music_player.h"
#include "music.h"
#include "event.h"
#include <stdbool.h>
#include <stdint.h>

// Playback context
static const Song *current_song = NULL;   // currently loaded song
static uint16_t note_index = 0;           // index of current note
static uint32_t note_start_time = 0;      // SysTick when current note began
static uint32_t note_duration = 0;        // duration of current note (ms)
static PlayerState player_state = PLAYER_STOPPED;

/*
 * Initialize the music player. SysTick is already running from LaunchPadInit().
 */
void MusicPlayerInit(void)
{
    // Nothing to do – SysTick is configured by the event scheduler.
}

/*
 * Load a song and start playback from the beginning.
 */
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
        MusicTurnOffLed();                // LED off during silence
    } else {
        MusicSetBuzzer(pitch, 2);         // volume high
        MusicSetLed(pitch, 2);
    }
}

/*
 * Pause playback (keep current position).
 */
void MusicPlayerPause(void)
{
    if (player_state == PLAYER_PLAYING) {
        player_state = PLAYER_PAUSED;
        MusicTurnOffBuzzer();
        MusicTurnOffLed();
    }
}

/*
 * Resume playback from paused state.
 */
void MusicPlayerResume(void)
{
    if (player_state == PLAYER_PAUSED && current_song) {
        player_state = PLAYER_PLAYING;
        note_start_time = EventGetCurrentTime();  // restart duration counting
        uint8_t pitch = current_song->notes[note_index].pitch;
        if (pitch == PITCH_REST) {
            MusicTurnOffBuzzer();
            MusicTurnOffLed();
        } else {
            MusicSetBuzzer(pitch, 2);
            MusicSetLed(pitch, 2);
        }
    }
}

/*
 * Stop playback and reset position.
 */
void MusicPlayerStop(void)
{
    player_state = PLAYER_STOPPED;
    current_song = NULL;
    note_index = 0;
    MusicTurnOffBuzzer();
    MusicTurnOffLed();
}

/*
 * Return the current player state.
 */
PlayerState MusicPlayerGetState(void)
{
    return player_state;
}

/*
 * Return the current note index (for debugging).
 */
uint16_t MusicPlayerGetCurrentNoteIndex(void)
{
    return note_index;
}

/*
 * Update the music player. Should be called periodically (e.g., every 10 ms).
 * Advances to the next note when the current note's duration has elapsed.
 */
void MusicPlayerUpdate(void)
{
    if (player_state != PLAYER_PLAYING || !current_song)
        return;

    uint32_t now = EventGetCurrentTime();
    if (now - note_start_time >= note_duration) {
        // Move to next note
        note_index++;
        if (note_index >= current_song->length) {
            // Song finished -> stop
            MusicPlayerStop();
            return;
        }

        // Start new note
        note_start_time = now;
        note_duration = current_song->notes[note_index].duration;
        uint8_t pitch = current_song->notes[note_index].pitch;
        if (pitch == PITCH_REST) {
            MusicTurnOffBuzzer();
            MusicTurnOffLed();
        } else {
            MusicSetBuzzer(pitch, 2);
            MusicSetLed(pitch, 2);
        }
    }
}
