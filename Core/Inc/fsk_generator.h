/*
 * fsk_generator.h
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#ifndef FSK_GENERATOR_H_
#define FSK_GENERATOR_H_

/* Includes */
#include "stm32u5xx_hal.h"
#include "main.h"

/* Constants */

/* Functions */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdma);
void startFskTransmission(void);
void stopFskTransmission(void);

/* Variables */
//extern DMA_QListTypeDef DACQueue;

#endif /* FSK_GENERATOR_H_ */
