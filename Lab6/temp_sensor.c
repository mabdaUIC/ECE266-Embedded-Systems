/*
 * temp_sensor.c  –  ECE 266 Lab 6, spring 2026
 * ADC0 sequencer #0, internal temperature sensor, processor trigger.
 * Created by Zhao Zhang.
 */

#include "temp_sensor.h"

/*
 * State layout – offsets MUST match temp_sensor_asm.asm:
 *   offset  0 :  callback_event  (Event *)    4 bytes
 *   offset  4 :  raw_data        (uint32_t)   4 bytes
 *   offset  8 :  new_input       (bool)       1 byte
 */
typedef struct {
    Event    *callback_event;
    uint32_t  raw_data;
    bool      new_input;
} TempSensorState;

TempSensorState temp_sensor;   /* non-static – referenced by temp_sensor_asm.asm */

void TsInit()
{
    temp_sensor.callback_event = NULL;
    temp_sensor.new_input      = false;

    /* Enable ADC0 and wait until ready before touching registers */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {}

    /* Sequencer #0: processor trigger, priority 0 */
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    /* Step 0: internal temperature sensor, interrupt on completion, end of seq */
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
                             ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    /* Register ISR, enable ADC-level + NVIC interrupts — done exactly ONCE */
    ADCIntRegister(ADC0_BASE, 0, TempSensorISR);
    ADCIntEnable(ADC0_BASE, 0);
    IntEnable(INT_ADC0SS0);

    /* Enable sequencer #0 — done exactly ONCE */
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
    float temp_C = 147.5f - (75.0f * 3.3f * (float)temp_sensor.raw_data / 4096.0f);
    temp_sensor.new_input = false;
    return temp_C * 1.8f + 32.0f;
}

void TsEventRegister(Event *event)
{
    temp_sensor.callback_event = event;
}
