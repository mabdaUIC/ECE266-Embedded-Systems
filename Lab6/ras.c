// ras.c – Driver for rotary angle sensor on J5 (PE2/AIN1), interrupt-driven
#include "ras.h"
#include <inc/hw_memmap.h>
#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>

typedef struct {
    Event *callback_event;
    uint32_t raw_data;
    bool new_input;
} RasState;

RasState ras_state;

void RasInit()
{
    // Enable GPIO port for RAS pin (PE2)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // Configure PE2 as analog input (AIN1)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    // Configure sequencer #1 for RAS
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);

    // Step 0: channel 1 (AIN1) for PE2, interrupt, end of sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0,
                             ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    // Register ISR (assembly)
    ADCIntRegister(ADC0_BASE, 1, RasISR);
    ADCIntEnable(ADC0_BASE, 1);

    ADCSequenceEnable(ADC0_BASE, 1);

    ras_state.callback_event = NULL;
    ras_state.new_input = false;
}

void RasTriggerReading()
{
    ADCProcessorTrigger(ADC0_BASE, 1);
}

bool RasDataReady()
{
    return ras_state.new_input;
}

uint32_t RasDataRead()
{
    uint32_t angle = (ras_state.raw_data * 180UL) / 4096UL;
    ras_state.new_input = false;
    return angle;
}

void RasEventRegister(Event *event)
{
    ras_state.callback_event = event;
}
