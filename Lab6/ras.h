/*
 * ras.h  –  ECE 266 Lab 6, spring 2026
 *
 * IMPORTANT: this header is included by ras_asm.asm via .cdecls "ras.h"
 * so it MUST include every header needed by the assembly (hw_memmap.h for
 * ADC0_BASE, driverlib/adc.h for ADCSequenceDataGet / ADCIntClear, etc.)
 * This mirrors the pattern used by temp_sensor.h / temp_sensor_asm.asm.
 */

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
