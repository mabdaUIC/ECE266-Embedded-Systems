/*
 * music_buzzer.c – Control buzzer using PWM for music notes
 *
 * The buzzer pitch is determined by the PWM frequency (period).
 * The loudness (volume) is determined by the PWM duty cycle (pulse width).
 *
 * The note frequencies are converted to PWM periods using:
 *   period = 50,000,000 / frequency
 *
 * Volume levels are implemented as duty cycles: low = 2%, medium = 10%, high = 50%.
 */

#include "music.h"
#include "pwm_buzzer.h"

// Pre computed PWM periods (in clock cycles) for C4 through B4.
// Formula: period = 50,000,000 / frequency (rounded to nearest integer)
static const uint32_t note_periods[NUM_PITCH] = {
    191110,     // C4 (261.63 Hz)
    170265,     // D4 (293.66 Hz)
    151685,     // E4 (329.63 Hz)
    143172,     // F4 (349.23 Hz)
    127551,     // G4 (392.00 Hz)
    113636,     // A4 (440.00 Hz)
    101239      // B4 (493.88 Hz)
};

<<<<<<< HEAD
// LED brightness settings defined as PWM period and PWM pulse width
typedef struct
{
    int period;
    int pulse_width;
} LEDBrightnessSetting;

static const float volume_levels[NUM_VOLUME_LEVEL] =
{
   0.01f,              // level 0 quiet
   0.05f,              // level 1 medium
   0.25f              // level 2 loud
=======
// Duty cycles for the three volume levels (fraction of period)
static const float volume_levels[NUM_VOLUME_LEVEL] = {
    0.01f,   // low   – 2% duty cycle (quiet)
    0.05f,   // medium – 10% duty cycle
    0.25f    // high  – 50% duty cycle (loud)
>>>>>>> 8ce9a1fe9e374a40684e762e4bb625c8f6c024cf
};

/*
 * Initialize the buzzer PWM hardware.
 */
void MusicInitBuzzer(void)
{
    PwmBuzzerInit();
}

/*
 * Set buzzer pitch and volume based on selected note and volume level.
 * pitch  – index 0..6 (C4 to B4)
 * volume – index 0..2 (low, medium, high)
 */
void MusicSetBuzzer(int pitch, int volume)
{
    uint32_t period = note_periods[pitch];               // PWM period in clock cycles
    uint32_t pulse_width = (uint32_t)(period * volume_levels[volume]); // pulse width
    PwmBuzzerSet(period, pulse_width);
}

/*
 * Turn off the buzzer (silence) by setting pulse width to zero.
 */
void MusicTurnOffBuzzer(void)
{
    PwmBuzzerSet(1000, 0);   // any period, zero pulse width = no sound
}
