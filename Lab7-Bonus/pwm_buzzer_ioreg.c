/*
 * pwm_buzzer_ioreg.c – Buzzer PWM using direct I/O register access (no TivaWare)
 *
 * Hardware: PC5, Wide Timer 0 sub timer B (WT0CCP1)
 *
 * All timer and GPIO configuration is performed by writing to memory-mapped registers.
 * This file is a drop-in replacement for the original pwm_buzzer.c from Lab 7.
 */

#include <stdint.h>

// ------------------------------------------------------------------
// Hardware base addresses (from TM4C123GH6PM datasheet)
// ------------------------------------------------------------------
#define WTIMER0_BASE        0x40036000U      // Wide Timer 0
#define GPIO_PORTC_BASE     0x40006000U      // GPIO Port C

// ------------------------------------------------------------------
// Clock gating registers (System Control module)
// ------------------------------------------------------------------
#define SYSCTL_RCGCWTIMER   0x400FE65CU      // RCGCWTIMER
#define SYSCTL_RCGCGPIO     0x400FE608U      // RCGCGPIO

// Clock enable bits
#define SYSCTL_RCGCWTIMER_R0    0x00000001U  // WTIMER0 clock enable
#define SYSCTL_RCGCGPIO_R2      0x00000004U  // Port C clock enable (bit 2)

// ------------------------------------------------------------------
// GPIO register offsets
// ------------------------------------------------------------------
#define GPIO_O_DIR          0x00000400U      // Direction
#define GPIO_O_AFSEL        0x00000420U      // Alternate function select
#define GPIO_O_DEN          0x0000051CU      // Digital enable
#define GPIO_O_PCTL         0x0000052CU      // Port control
#define GPIO_O_DR8R         0x00000508U      // 8mA drive strength

// GPIO Pin mask for PC5
#define GPIO_PIN_5          0x00000020U

// GPIO PCTL field for PC5 (bits 23:20)
#define GPIO_PCTL_PC5_M     0x00F00000U      // Mask for PC5 field
#define GPIO_PCTL_PC5_WT0CCP1 0x00700000U    // 0x7 << 20 -> WT0CCP1

// ------------------------------------------------------------------
// Timer register offsets
// ------------------------------------------------------------------
#define TIMER_O_CFG         0x00000000U      // Configuration
#define TIMER_O_TBMR        0x00000028U      // Timer B mode
#define TIMER_O_CTL         0x0000000CU      // Control
#define TIMER_O_TBILR       0x0000002CU      // Timer B interval load (period)
#define TIMER_O_TBMATCHR    0x00000034U      // Timer B match (pulse width)

// Timer configuration constants
#define TIMER_CFG_SPLIT_PAIR    0x00000004U  // Split 64-bit into two 32-bit timers
#define TIMER_TBMR_TBMR_PWM     0x00000002U  // PWM mode for timer B
#define TIMER_TBMR_TBCDIR       0x00000008U  // Count down direction
#define TIMER_CTL_TBEN          0x00000100U  // Timer B enable
#define TIMER_CTL_TBPWML        0x00004000U  // Timer B PWM output invert (bit 14)

// ------------------------------------------------------------------
// Helper: enable a peripheral clock by writing to RCGC register
// ------------------------------------------------------------------
static inline void enablePeripheralClock(uint32_t reg_addr, uint32_t bit_mask)
{
    volatile uint32_t *reg = (uint32_t *)reg_addr;
    *reg |= bit_mask;
    (void)*reg;  // small delay to allow clock to stabilize
}

// ------------------------------------------------------------------
// Initialize the buzzer PWM hardware (replaces PwmBuzzerInit from Lab 7)
// ------------------------------------------------------------------
void PwmBuzzerInit(void)
{
    // 1. Enable clocks for Wide Timer 0 and GPIO Port C
    enablePeripheralClock(SYSCTL_RCGCWTIMER, SYSCTL_RCGCWTIMER_R0);
    enablePeripheralClock(SYSCTL_RCGCGPIO,   SYSCTL_RCGCGPIO_R2);

    // 2. Configure PC5 as a timer (PWM) output pin
    volatile uint32_t *gpio_dir   = (uint32_t *)(GPIO_PORTC_BASE + GPIO_O_DIR);
    volatile uint32_t *gpio_afsel = (uint32_t *)(GPIO_PORTC_BASE + GPIO_O_AFSEL);
    volatile uint32_t *gpio_den   = (uint32_t *)(GPIO_PORTC_BASE + GPIO_O_DEN);
    volatile uint32_t *gpio_dr8r  = (uint32_t *)(GPIO_PORTC_BASE + GPIO_O_DR8R);
    volatile uint32_t *gpio_pctl  = (uint32_t *)(GPIO_PORTC_BASE + GPIO_O_PCTL);

    *gpio_dir   |= GPIO_PIN_5;           // Set as output
    *gpio_afsel |= GPIO_PIN_5;           // Enable alternate function
    *gpio_den   |= GPIO_PIN_5;           // Digital enable
    *gpio_dr8r  |= GPIO_PIN_5;           // 8mA drive strength (like GPIOPinTypeTimer)

    // Set PC5 to use WT0CCP1 (PMC7 = 0x7)
    uint32_t pctl_val = *gpio_pctl;
    pctl_val &= ~GPIO_PCTL_PC5_M;        // Clear the 4 bits for PC5
    pctl_val |= GPIO_PCTL_PC5_WT0CCP1;   // Set to 0x7 (WT0CCP1)
    *gpio_pctl = pctl_val;

    // 3. Configure Wide Timer 0, sub timer B for PWM mode
    volatile uint32_t *timer_cfg  = (uint32_t *)(WTIMER0_BASE + TIMER_O_CFG);
    volatile uint32_t *timer_tbmr = (uint32_t *)(WTIMER0_BASE + TIMER_O_TBMR);
    volatile uint32_t *timer_ctl  = (uint32_t *)(WTIMER0_BASE + TIMER_O_CTL);

    // Split the 64-bit timer into two independent 32-bit timers (A and B)
    *timer_cfg = TIMER_CFG_SPLIT_PAIR;

    // Set PWM mode AND count-down direction (critical for correct PWM)
    *timer_tbmr = TIMER_TBMR_TBMR_PWM | TIMER_TBMR_TBCDIR;

    // Invert output: set TBPWML bit so that the match register value equals the pulse width
    *timer_ctl |= TIMER_CTL_TBPWML;

    // 4. Enable sub timer B
    *timer_ctl |= TIMER_CTL_TBEN;
}

// ------------------------------------------------------------------
// Set PWM period and pulse width (replaces PwmBuzzerSet from Lab 7)
// ------------------------------------------------------------------
void PwmBuzzerSet(uint32_t periodCycles, uint32_t pulseWidthCycles)
{
    volatile uint32_t *timer_tbilr   = (uint32_t *)(WTIMER0_BASE + TIMER_O_TBILR);
    volatile uint32_t *timer_tbmatchr = (uint32_t *)(WTIMER0_BASE + TIMER_O_TBMATCHR);
    volatile uint32_t *timer_ctl      = (uint32_t *)(WTIMER0_BASE + TIMER_O_CTL);

    // Disable the timer to avoid glitches while updating
    *timer_ctl &= ~TIMER_CTL_TBEN;

    // Write the period (load) and pulse width (match) registers.
    // The hardware expects (value - 1).
    *timer_tbilr   = periodCycles - 1;
    *timer_tbmatchr = pulseWidthCycles - 1;

    // Re-enable the timer
    *timer_ctl |= TIMER_CTL_TBEN;
}
