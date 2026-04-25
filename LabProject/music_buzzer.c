/*
 * music_buzzer.c – Control buzzer using PWM for music notes.
 *
 * The loudness (volume) is now determined by a volume percentage (0-100)
 * that scales the duty cycle linearly.
 */

#include "music.h"
#include "pwm_buzzer.h"

// Pre-computed PWM periods (in clock cycles) for C4 through B4.
static const uint32_t note_periods[NUM_PITCH] = {
    191110,     // C4 (261.63 Hz)
    170265,     // D4 (293.66 Hz)
    151685,     // E4 (329.63 Hz)
    143172,     // F4 (349.23 Hz)
    127551,     // G4 (392.00 Hz)
    113636,     // A4 (440.00 Hz)
    101239      // B4 (493.88 Hz)
};

void MusicInitBuzzer(void)
{
    PwmBuzzerInit();
}

/*
 * Set buzzer pitch and volume.
 *   pitch          – index 0..6 (C4 to B4)
 *   volume_percent – 0 = silent, 100 = full volume
 */
void MusicSetBuzzer(int pitch, int volume_percent)
{
    uint32_t period = note_periods[pitch];
    // Pulse width: linear scaling, duty = volume_percent %
    uint32_t pulse_width = (period * volume_percent) / 100;
    PwmBuzzerSet(period, pulse_width);
}

void MusicTurnOffBuzzer(void)
{
    PwmBuzzerSet(1000, 0);   // zero pulse width = silence
}
