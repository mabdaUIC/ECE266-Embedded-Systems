#ifndef MUSIC_PLAYER_H_
#define MUSIC_PLAYER_H_

#include <stdbool.h>
#include <stdint.h>
#include "song_library.h"

#define PITCH_REST  255   // Special pitch value for silence (rest)

typedef enum {
    PLAYER_STOPPED,
    PLAYER_PLAYING,
    PLAYER_PAUSED
} PlayerState;

void MusicPlayerInit(void);
void MusicPlayerPlaySong(const Song *song);
void MusicPlayerPause(void);
void MusicPlayerResume(void);
void MusicPlayerStop(void);
PlayerState MusicPlayerGetState(void);
uint16_t MusicPlayerGetCurrentNoteIndex(void);
void MusicPlayerUpdate(void);

#endif /* MUSIC_PLAYER_H_ */
