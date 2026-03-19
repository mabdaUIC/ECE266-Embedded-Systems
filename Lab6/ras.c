#include <stdint.h>
#include <stdbool.h>
#include "ras.h"
#include <inc/hw_memmap.h>
#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>

void RasInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
<<<<<<< HEAD
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    // Register and enable interrupt
=======
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);
>>>>>>> 50135634314b86c569dd86edaeacc0ac3740f9b7
    ADCIntRegister(ADC0_BASE, 1, RasISR);
    ADCIntEnable(ADC0_BASE, 1);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCIntClear(ADC0_BASE, 1);
}

void RasTriggerReading(void) {
    ADCProcessorTrigger(ADC0_BASE, 1);
}
