/*
 * fsk_generator.h
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#ifndef FSK_GENERATOR_H_
#define FSK_GENERATOR_H_

#include "stm32u5xx_hal.h"
#include "main.h"

void startFskTransmission(void);

extern DAC_HandleTypeDef hdac1;

#endif /* FSK_GENERATOR_H_ */
