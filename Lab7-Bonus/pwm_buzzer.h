#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdint.h>

void PwmBuzzerInit(void);
void PwmBuzzerSet(uint32_t periodCycles, uint32_t pulseWidthCycles);

#endif /* PWM_BUZZER_H_ */
