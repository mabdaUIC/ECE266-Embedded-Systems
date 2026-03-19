/*
 * temp_sensor.c – Driver for internal temperature sensor
 */
#include "temp_sensor.h"

typedef struct {
    Event *callback_event;
    uint32_t raw_data;
    bool new_input;
} TempSensorState;

TempSensorState temp_sensor;

void TsInit()
{
    temp_sensor.callback_event = NULL;
    temp_sensor.new_input = false;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    ADCIntRegister(ADC0_BASE, 0, TempSensorISR);
    ADCIntEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 0);
}

void TsTriggerReading()
{
    ADCProcessorTrigger(ADC0_BASE, 0);
}

bool TsDataReady()
{
    return temp_sensor.new_input;
}

float TsDataRead()
{
    float temp_C = 147.5 - (75 * 3.3 * (float) temp_sensor.raw_data / 4096);
    temp_sensor.new_input = false;
    return temp_C * 1.8 + 32;
}

void TsEventRegister(Event *event)
{
    temp_sensor.callback_event = event;
}
