/*
 * lab7_main.c – Main program for Lab 7 (bonus: direct register PWM for buzzer)
 *
 * Plays a simple melody using the buzzer on PC5.
 * System clock is set to 80 MHz via the PLL.
 */

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"      // For SysCtlClockSet()
#include "pwm_buzzer.h"            // For PwmBuzzerInit, PwmBuzzerSet
#include "music_buzzer.h"          // For melody and note definitions

int main(void)
{

    int i;  // C89 requires loop variable declared at top

    // Set system clock to 80 MHz (using PLL with 16 MHz crystal)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Initialize the buzzer PWM hardware (direct register access)
    PwmBuzzerInit();

    // Play the melody forever
    while (1)
    {
        for (i = 0; i < MELODY_LENGTH; i++)
        {
            uint32_t frequency = melody[i].frequency;
            uint32_t duration  = melody[i].duration;

            if (frequency == 0)
            {
                // Silence: turn off PWM by setting duty cycle to 0
                PwmBuzzerSet(1, 0);
            }
            else
            {
                // Calculate period cycles for 80 MHz clock
                uint32_t periodCycles = 80000000 / frequency;
                uint32_t pulseWidthCycles = periodCycles / 2; // 50% duty cycle
                PwmBuzzerSet(periodCycles, pulseWidthCycles);
            }

            // Wait for the note duration (SysCtlDelay uses cycles)
            SysCtlDelay(duration * (80000000 / 3 / 1000));
        }
    }
}
