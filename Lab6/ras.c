/*
 * ras.c  –  ECE 266 Lab 6, spring 2026
 * ADC0 sequencer #1, RAS on PE3 / AIN0 / J5, processor trigger.
 * ISR written in assembly: ras_asm.asm
 */

#include "ras.h"

/*
 * State layout – offsets MUST match ras_asm.asm:
 *   offset  0 :  callback_event  (Event *)    4 bytes
 *   offset  4 :  raw_data        (uint32_t)   4 bytes
 *   offset  8 :  new_input       (bool)       1 byte
 */
typedef struct {
    Event    *callback_event;
    uint32_t  raw_data;
    bool      new_input;
} RasState;

RasState ras_state;   /* non-static – referenced by ras_asm.asm via .ref */

void RasInit(void)
{
    ras_state.callback_event = NULL;
    ras_state.new_input      = false;

    /* ── GPIO Port E: PE3 = AIN0 = J5 signal pin ──────────────────────── */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);   /* disconnect digital buffer */

    /* ── ADC0: already enabled by TsInit; call is idempotent ─────────── */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {}

    /* Sequencer #1: processor trigger, priority 0 */
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    /* Step 0: AIN0 (PE3), interrupt on completion, end of sequence */
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0,
                             ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    /* Register assembly ISR, enable ADC-level interrupt, enable NVIC */
    ADCIntRegister(ADC0_BASE, 1, RasISR);
    ADCIntEnable(ADC0_BASE, 1);
    IntEnable(INT_ADC0SS1);

    /* Enable sequencer #1 — done exactly ONCE */
    ADCSequenceEnable(ADC0_BASE, 1);
}

void RasTriggerReading(void)
{
    ADCProcessorTrigger(ADC0_BASE, 1);
}

bool RasDataReady(void)
{
    return ras_state.new_input;
}

uint16_t RasDataRead(void)
{
    uint16_t angle = (uint16_t)((ras_state.raw_data * 180) / 4095);
    ras_state.new_input = false;
    return angle;
}

void RasEventRegister(Event *event)
{
    ras_state.callback_event = event;
}
