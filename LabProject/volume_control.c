/*
 * volume_control.c - ADC volume control using potentiometer on PD2 (J7)
 *
 * J7 Grove connector (custom pinout): PD2 (white, AIN5) for sensor output.
 */

#include "volume_control.h"
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "inc/hw_memmap.h"

// ---------- Pin definitions for PD2 (AIN5) ----------
#define VOL_ADC_BASE        ADC0_BASE
#define VOL_ADC_SEQUENCER   3               // Sequencer 3 (single sample)
#define VOL_ADC_CHANNEL     ADC_CTL_CH5     // PD2 = AIN5
#define VOL_PORT_PERIPH     SYSCTL_PERIPH_GPIOD
#define VOL_GPIO_PORT       GPIO_PORTD_BASE
#define VOL_GPIO_PIN        GPIO_PIN_2      // PD2

void VolumeControlInit(void)
{
    // Enable ADC0 and GPIO Port D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(VOL_PORT_PERIPH);

    // Configure PD2 as analog input
    GPIOPinTypeADC(VOL_GPIO_PORT, VOL_GPIO_PIN);

    // Set up sequencer 3: processor trigger, single sample, interrupt enabled
    ADCSequenceConfigure(VOL_ADC_BASE, VOL_ADC_SEQUENCER,
                         ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(VOL_ADC_BASE, VOL_ADC_SEQUENCER, 0,
                             VOL_ADC_CHANNEL | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(VOL_ADC_BASE, VOL_ADC_SEQUENCER);
}

uint8_t VolumeControlGetLevel(void)
{
    uint32_t adcValue = 0;

    ADCIntClear(VOL_ADC_BASE, VOL_ADC_SEQUENCER);
    ADCProcessorTrigger(VOL_ADC_BASE, VOL_ADC_SEQUENCER);

    // Wait for conversion to complete
    while (!ADCIntStatus(VOL_ADC_BASE, VOL_ADC_SEQUENCER, false))
    {
    }

    ADCSequenceDataGet(VOL_ADC_BASE, VOL_ADC_SEQUENCER, &adcValue);

    // 12-bit ADC (0-4095) -> volume 0-100
    uint8_t level = (uint8_t)((adcValue * 100) / 4095);

    return level;
}
