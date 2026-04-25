/*
 * volume_control.h - ADC-based volume control using potentiometer on PE3 (J8)
 */

#ifndef VOLUME_CONTROL_H_
#define VOLUME_CONTROL_H_

#include <stdint.h>

// Initialize ADC for reading potentiometer on PE3 (AIN0)
void VolumeControlInit(void);

// Read current volume level (0-100)
uint8_t VolumeControlGetLevel(void);

#endif /* VOLUME_CONTROL_H_ */
