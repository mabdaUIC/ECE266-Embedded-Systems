/*
 * pwm_buzzer.c – Low-level PWM control for the buzzer
 *
 * Hardware: PC5, Wide Timer 0 sub-timer B (WT0CCP1)
 * This is the correct pin for J17 on the Grove Base BoosterPack.
 */

#include "pwm_buzzer.h"

// Correct hardware definitions for buzzer on PC5 (J1p7)
#define BUZZER_TIMER_BASE   WTIMER0_BASE
#define BUZZER_TIMER        TIMER_B              // Changed to TIMER_B
#define BUZZER_GPIO_BASE    GPIO_PORTC_BASE
#define BUZZER_GPIO_PIN     GPIO_PIN_5           // Changed to GPIO_PIN_5
#define BUZZER_GPIO_CFG     GPIO_PC5_WT0CCP1    // Changed to GPIO_PC5_WT0CCP1

void PwmBuzzerInit(void)
{
    // Enable clocks for Wide Timer 0 and GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC5 as timer (PWM) output
    GPIOPinTypeTimer(BUZZER_GPIO_BASE, BUZZER_GPIO_PIN);
    GPIOPinConfigure(BUZZER_GPIO_CFG);

    // Split the 32/64 bit timer into two sub timers, sub timer B in PWM mode
    TimerConfigure(BUZZER_TIMER_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);

    // Invert output so that Match register directly gives pulse width
    TimerControlLevel(BUZZER_TIMER_BASE, BUZZER_TIMER, true);

    // Enable the timer
    TimerEnable(BUZZER_TIMER_BASE, BUZZER_TIMER);
}

void PwmBuzzerSet(uint32_t periodCycles, uint32_t pulseWidthCycles)
{
    // Disable timer briefly to avoid glitches during update
    TimerDisable(BUZZER_TIMER_BASE, BUZZER_TIMER);

    // Set load (period) and match (pulse width) registers
    TimerLoadSet(BUZZER_TIMER_BASE, BUZZER_TIMER, periodCycles - 1);
    TimerMatchSet(BUZZER_TIMER_BASE, BUZZER_TIMER, pulseWidthCycles - 1);

    // Re enable timer
    TimerEnable(BUZZER_TIMER_BASE, BUZZER_TIMER);
}
