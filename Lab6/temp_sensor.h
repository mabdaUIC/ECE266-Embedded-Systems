#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/adc.h>
#include "launchpad.h"

void TsInit();
void TsTriggerReading();
bool TsDataReady();
float TsDataRead();
void TsEventRegister(Event *event);
void TempSensorISR();

#endif /* TEMP_SENSOR_H_ */
