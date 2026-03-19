#ifndef RAS_H_
#define RAS_H_

#include <stdint.h>
#include <stdbool.h>

void RasInit(void);
void RasTriggerReading(void);
void RasISR(void);   // assembly ISR

#endif
