/*
 * lab6_main.c – For J5 (PE2/AIN1) with full debug output.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

void EventTick(void) { }

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

static float g_temperature = 75.8;
static uint32_t g_angle = 127;

uint8_t safe_digit(int d)
{
    if (d >= 0 && d <= 9) return seg7_codes[d];
    else return seg7_codes[10];
}

void DisplayTemperature(float temp)
{
    int i;
    int temp_int = (int)temp;
    int temp_frac = (int)(temp * 10) % 10;
    int tens = temp_int / 10;
    int ones = temp_int % 10;

    uint8_t codes[4];
    codes[3] = safe_digit(tens);
    codes[2] = safe_digit(ones);
    codes[1] = safe_digit(temp_frac);
    codes[0] = seg7_codes[10];

    for (i = 0; i < 4; i++) codes[i] |= 0x80;
    Seg7RawUpdate(codes);
}

void DisplayRAS(uint32_t angle)
{
    int hundreds = angle / 100;
    int tens = (angle / 10) % 10;
    int ones = angle % 10;

    uint8_t codes[4];
    codes[3] = seg7_codes[10];

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

int main(void)
{
    uint32_t sysClock;
    uint32_t raw_temp, raw_ras;
    float voltage;

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    sysClock = SysCtlClockGet();
    LaunchPadInit();

    // Blue LED
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    Seg7Init();
    uprintf("Seg7Init done\r\n");

    // Test pattern
    uint8_t test_codes[4] = {0b00000110, 0b01011011, 0b01001111, 0b01100110};
    uprintf("Display test pattern 1234...\r\n");
    Seg7RawUpdate(test_codes);
    SysCtlDelay(sysClock * 2);

    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // Temperature on sequencer 0
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_TS | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 0);

    // RAS on J5: PE2 (AIN1) sequencer 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1 | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);

    uprintf("ADC initialized (RAS on J5).\r\n");
    uprintf("Entering main loop.\r\n");

    bool show_temperature = true;
    int loop_count = 0;

    while (1)
    {
        loop_count++;
        uprintf("\n--- Loop %d ---\r\n", loop_count);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(sysClock / 500);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

        // Temperature
        ADCProcessorTrigger(ADC0_BASE, 0);
        SysCtlDelay(sysClock / 100);
        if (ADCSequenceDataGet(ADC0_BASE, 0, &raw_temp) != 0)
        {
            float temp_C = 147.5 - (75 * 3.3f * raw_temp / 4096.0f);
            g_temperature = temp_C * 1.8f + 32.0f;
            uprintf("Temp raw: %u\n", raw_temp);
        }

        // RAS
        ADCProcessorTrigger(ADC0_BASE, 1);
        SysCtlDelay(sysClock / 100);
        if (ADCSequenceDataGet(ADC0_BASE, 1, &raw_ras) != 0)
        {
            voltage = (raw_ras * 3.3f) / 4096.0f;
            g_angle = (uint32_t)((raw_ras * 180.0f) / 4096.0f + 0.5f);
            uprintf("RAS raw: %4u, Voltage: %.3f V, Angle: %u\n", raw_ras, voltage, g_angle);
        }

        // Required output
        int temp10 = (int)(g_temperature * 10);
        uprintf("RAS Reading: %u\n", g_angle);
        uprintf("Temperature Sensor Reading: %u.%u\n", temp10 / 10, temp10 % 10);

        // Display
        if (show_temperature)
            DisplayTemperature(g_temperature);
        else
            DisplayRAS(g_angle);

        show_temperature = !show_temperature;
        SysCtlDelay(sysClock * 2);
    }
}
