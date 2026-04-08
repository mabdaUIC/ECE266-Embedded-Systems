/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), spring 2026
 *
 * music_led.c: Set LED for a music note
 *
 * Created by Zhao Zhang
 */

#include "music.h"
#include "pwm_buzzer.h"

/*
 * Color and brightness settings of the three sub-LEDs for a given
 * pitch level and volume level.
 */

// LED color settings for a given pitch
typedef struct
{
    float red;          // weight for red color
    float blue;         // weight for blue color
    float green;        // weight for green color
} LEDColorSetting;

static const uint32_t note_periods[NUM_PITCH] =
{
    // weights for red, blue, and green colors
    191110,     // C4
    170265,        // D4
    151685,        // E4
    143172,        // F4
    127551,        // G4
    113636,        // A4
    101239,        // B4
};

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
};

/*
 * Initialize the LEDs for music note playing
 */
void MusicInitBuzzer()
{
    PwmBuzzerInit();
}

/*
 * Set the LED for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for the three sub-LEDs,
 * and then call LedPwmSet() to set those parameters.
 */
void MusicSetBuzzer(int pitch, int volume)
{
    uint32_t period = note_periods[pitch];

    uint32_t pulse_width = (uint32_t)(period * volume_levels[volume]);


    PwmBuzzerSet(period, pulse_width);
}

/*
 * Turn off the LED
 */
void MusicTurnOffBuzzer()
{
    PwmBuzzerSet(1000, 0);
}
