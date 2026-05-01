/*
 * music_buzzer.h – Buzzer interface
 */

#ifndef MUSIC_BUZZER_H_
#define MUSIC_BUZZER_H_

#include "launchpad.h"

void MusicInitBuzzer(void);

// Set buzzer pitch and volume percentage (0-100)
void MusicSetBuzzer(int pitch, int volume_percent);

void MusicTurnOffBuzzer(void);

#endif
