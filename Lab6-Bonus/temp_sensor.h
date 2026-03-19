#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>

void TsInit(void);
void TsTriggerReading(void);
void TempSensorISR(void);   // assembly ISR

#endif
