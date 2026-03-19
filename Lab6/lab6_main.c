/*
 * lab6_main.c – Interrupt driven version with direct ISR flag handling
 * ECE 266 Lab 6, Spring 2026
 *
 * RAS on J5 (PE2/AIN1), 7 seg on J10 (PA6, PA7)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"
#include "ras.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"

// Global variables shared with ISRs (volatile because accessed by interrupts)
volatile uint32_t g_temp_raw = 0;
volatile uint32_t g_ras_raw = 0;
volatile bool    g_temp_ready = false;
volatile bool    g_ras_ready = false;

// 7 segment encoding table (index 10 = blank)
static const uint8_t seg7_codes[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b00000000  // blank
};

// Helper: safe digit extraction
uint8_t safe_digit(int d) {
    if (d >= 0 && d <= 9) return seg7_codes[d];
    return seg7_codes[10];
}

// Display temperature on left three digits with colon on, rightmost blank
void DisplayTemperature(float temp) {
    int i;
    int temp_int = (int)temp;
    int temp_frac = (int)(temp * 10) % 10;
    int tens = temp_int / 10;
    int ones = temp_int % 10;
    uint8_t codes[4];

    codes[3] = safe_digit(tens);      // leftmost
    codes[2] = safe_digit(ones);
    codes[1] = safe_digit(temp_frac);
    codes[0] = seg7_codes[10];        // blank rightmost

    for (i = 0; i < 4; i++) codes[i] |= 0x80; // colon on
    Seg7RawUpdate(codes);
}

// Display RAS angle on right three digits, no leading zeros, colon off, leftmost blank
void DisplayRAS(uint32_t angle) {
    int hundreds = angle / 100;
    int tens = (angle / 10) % 10;
    int ones = angle % 10;
    uint8_t codes[4];

    codes[3] = seg7_codes[10];        // leftmost blank

    if (hundreds == 0) {
        codes[2] = seg7_codes[10];
        if (tens == 0) {
            codes[1] = seg7_codes[10];
        } else {
            codes[1] = safe_digit(tens);
        }
    } else {
        codes[2] = safe_digit(hundreds);
        codes[1] = safe_digit(tens);
    }
    codes[0] = safe_digit(ones);
    Seg7RawUpdate(codes);
}

// Convert raw temperature value to Fahrenheit
float RawToFahrenheit(uint32_t raw) {
    float temp_C = 147.5 - (75 * 3.3f * raw / 4096.0f);
    return temp_C * 1.8f + 32.0f;
}

// Convert raw RAS value to angle (0 180°) with rounding
uint32_t RawToAngle(uint32_t raw) {
    return (raw * 180UL + 2048) / 4096;
}

int main(void) {
    uint32_t sysClock;
    bool show_temperature = true;
    float current_temp = 75.8;
    uint32_t current_angle = 127;

    // Set system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    sysClock = SysCtlClockGet();

    // Initialize UART and basic system
    LaunchPadInit();

    // Enable blue LED for heartbeat
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Initialize 7 segment display
    Seg7Init();
    uprintf("Seg7Init done\r\n");

    // Test pattern 1234 for 2 seconds
    uint8_t test_codes[4] = {0b00000110, 0b01011011, 0b01001111, 0b01100110};
    uprintf("Display test pattern 1234...\r\n");
    Seg7RawUpdate(test_codes);
    SysCtlDelay(sysClock * 2);

    // Initialize sensors (interrupt driven)
    TsInit();
    RasInit();
    uprintf("Sensors initialized.\r\n");

    // Enable global interrupts
    IntMasterEnable();
    uprintf("Global interrupts enabled.\r\n");

    uprintf("Entering main loop.\r\n");

    while (1) {
        // Heartbeat: brief blue LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(sysClock / 500);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

        // Trigger new conversions
        TsTriggerReading();
        RasTriggerReading();

        // Wait for data with timeout (prevents hanging)
        uint32_t timeout = sysClock / 10; // 0.1 second
        while (!g_temp_ready && !g_ras_ready && timeout-- > 0) {
            __asm(" nop");
        }

        // Process temperature if ready
        if (g_temp_ready) {
            current_temp = RawToFahrenheit(g_temp_raw);
            g_temp_ready = false;
        }

        // Process RAS if ready
        if (g_ras_ready) {
            current_angle = RawToAngle(g_ras_raw);
            g_ras_ready = false;
        }

        // Print readings (lab requires both printed each loop)
        int temp10 = (int)(current_temp * 10);
        uprintf("RAS Reading: %u\r\n", current_angle);
        uprintf("Temperature Sensor Reading: %u.%u\r\n", temp10 / 10, temp10 % 10);

        // Update display (alternate every 2 seconds)
        if (show_temperature) {
            DisplayTemperature(current_temp);
        } else {
            DisplayRAS(current_angle);
        }
        show_temperature = !show_temperature;

        // Wait 2 seconds
        SysCtlDelay(sysClock * 2);
    }
}
