/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), spring 2026
 *
 * music_buzzer.h – Buzzer interface
 */

#ifndef MUSIC_BUZZER_H_
#define MUSIC_BUZZER_H_

#include "launchpad.h"

// Initialize the buzzer
void MusicInitBuzzer(void);

// Set the buzzer for a given pitch and volume percentage (0-100).
// 0 = silent, 100 = full loudness.
void MusicSetBuzzer(int pitch, int volume_percent);

// Turn off buzzer
void MusicTurnOffBuzzer(void);

#endif /* MUSIC_BUZZER_H_ */
