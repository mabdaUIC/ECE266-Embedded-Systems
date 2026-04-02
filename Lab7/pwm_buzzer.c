/*
 * pwm_buzzer.c – Low level PWM control for the buzzer
 *
 * Hardware: PC4, Wide Timer 0 sub timer A (WT0CCP0)
 *
 * PWM mode: count down, output inverted so that Match register = pulse width.
 */

#include "pwm_buzzer.h"

// Hardware definitions for buzzer on PC4 (Grove baseboard J17 often uses this)
#define BUZZER_TIMER_BASE   WTIMER0_BASE
#define BUZZER_TIMER        TIMER_A
#define BUZZER_GPIO_BASE    GPIO_PORTC_BASE
#define BUZZER_GPIO_PIN     GPIO_PIN_4
#define BUZZER_GPIO_CFG     GPIO_PC4_WT0CCP0

void PwmBuzzerInit(void)
{
    // Enable clocks for Wide Timer 0 and GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC4 as timer (PWM) output
    GPIOPinTypeTimer(BUZZER_GPIO_BASE, BUZZER_GPIO_PIN);
    GPIOPinConfigure(BUZZER_GPIO_CFG);

    // Split the 32/64 bit timer into two sub timers, sub timer A in PWM mode
    TimerConfigure(BUZZER_TIMER_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);

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
    // Note: hardware expects (value - 1)
    TimerLoadSet(BUZZER_TIMER_BASE, BUZZER_TIMER, periodCycles - 1);
    TimerMatchSet(BUZZER_TIMER_BASE, BUZZER_TIMER, pulseWidthCycles - 1);

    // Re enable timer
    TimerEnable(BUZZER_TIMER_BASE, BUZZER_TIMER);
}
