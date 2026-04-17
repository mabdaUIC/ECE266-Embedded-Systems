#ifndef MUSIC_PLAYER_H_
#define MUSIC_PLAYER_H_

#include <stdbool.h>
#include <stdint.h>
#include "song_library.h"

// Player states
typedef enum {
    PLAYER_STOPPED,
    PLAYER_PLAYING,
    PLAYER_PAUSED
} PlayerState;

// Initialize the music player (set up timer interrupt for timing)
void MusicPlayerInit(void);

// Load a song and start playing from the beginning
void MusicPlayerPlaySong(const Song *song);

// Pause playback (keep current position)
void MusicPlayerPause(void);

// Resume playback from paused state
void MusicPlayerResume(void);

// Stop playback (reset position)
void MusicPlayerStop(void);

// Get current player state
PlayerState MusicPlayerGetState(void);

// Get current note index (for display or debugging)
uint16_t MusicPlayerGetCurrentNoteIndex(void);

// Call this periodically from main loop (or event handler) to update LED and speaker
// It will be called by the timer interrupt handler; you can also call it from EventExecute.
void MusicPlayerUpdate(void);

#endif /* MUSIC_PLAYER_H_ */
