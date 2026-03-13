#ifndef RAS_H_
#define RAS_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include "launchpad.h"

void RasInit(void);
void RasTriggerReading(void);
bool RasDataReady(void);
uint16_t RasDataRead(void);          // returns angle 0..180
void RasEventRegister(Event *event);
void RasISR(void);                   // assembly ISR in ras_asm.asm

#endif /* RAS_H_ */
