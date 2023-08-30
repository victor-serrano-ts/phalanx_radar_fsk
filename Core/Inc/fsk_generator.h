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

/* Types */
typedef struct {
	uint32_t freq_desviation; 		// 2MHz = 6710809
	uint8_t mod_pin;							// 7 = input
	uint8_t fsk_trigger_src;      // 1 = trigger A
	uint8_t trigger_src_def;			// 3 = MOD rising edge
}pll_fsk_config_t;

typedef enum {
	PLL_DISABLE = 0,
	PLL_ENABLE = 1
}pll_state_t;

/* Functions */
void setConfigPll(pll_fsk_config_t *config);
void setPllState(pll_state_t state);
void startFskTransmission(void);
void stopFskTransmission(void);

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdma);

/* Variables */


#endif /* FSK_GENERATOR_H_ */
