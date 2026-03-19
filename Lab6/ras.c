/*
 * ras.c – Interrupt driven driver for rotary angle sensor (J5, PE2/AIN1)
 * Uses sequencer 1, processor trigger, interrupt enabled.
 */

#include <stdint.h>
#include <stdbool.h>
#include "ras.h"
#include <inc/hw_memmap.h>
#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>

void RasInit(void) {
    // Enable GPIO for PE2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // Configure PE2 as analog input (AIN1)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // Configure sequencer 1 for RAS
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    // Register and enable interrupt
    ADCIntRegister(ADC0_BASE, 1, RasISR);
    ADCIntEnable(ADC0_BASE, 1);

    // Enable sequencer
    ADCSequenceEnable(ADC0_BASE, 1);

    // Clear any pending interrupts
    ADCIntClear(ADC0_BASE, 1);
}

void RasTriggerReading(void) {
    ADCProcessorTrigger(ADC0_BASE, 1);
}
