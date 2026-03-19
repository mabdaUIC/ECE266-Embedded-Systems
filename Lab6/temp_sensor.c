/*
 * temp_sensor.c – Interrupt driven driver for internal temperature sensor
 * Uses sequencer 0, processor trigger, interrupt enabled.
 */

#include <stdint.h>
#include <stdbool.h>
#include "temp_sensor.h"
#include <inc/hw_memmap.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>

void TsInit(void) {
    // Enable ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // Configure sequencer 0 for temperature sensor
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    // Register and enable interrupt
    ADCIntRegister(ADC0_BASE, 0, TempSensorISR);
    ADCIntEnable(ADC0_BASE, 0);

    // Enable sequencer
    ADCSequenceEnable(ADC0_BASE, 0);

    // Clear any pending interrupts
    ADCIntClear(ADC0_BASE, 0);
}

void TsTriggerReading(void) {
    ADCProcessorTrigger(ADC0_BASE, 0);
}
