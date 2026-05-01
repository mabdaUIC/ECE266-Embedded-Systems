/*
 * volume_control.h - ADC-based volume control using potentiometer on PD2 (J7)
 * Polling version: VolumeControlGetLevel() reads ADC on demand.
 */

#ifndef VOLUME_CONTROL_H_
#define VOLUME_CONTROL_H_

#include <stdint.h>

// Initialize ADC for reading potentiometer on PD2 (AIN5)
void VolumeControlInit(void);

// Read current volume level (0-100) – polling
uint8_t VolumeControlGetLevel(void);

#endif /* VOLUME_CONTROL_H_ */
