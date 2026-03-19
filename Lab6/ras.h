#ifndef RAS_H_
#define RAS_H_

#include <stdbool.h>
#include <stdint.h>
#include "launchpad.h"

void RasInit();
void RasTriggerReading();
bool RasDataReady();
uint32_t RasDataRead();
void RasEventRegister(Event *event);
void RasISR();   // assembly ISR

#endif
