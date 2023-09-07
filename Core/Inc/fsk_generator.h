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

/* Constants */

/* Types */

/** @brief Type of PLL configuration. */
typedef struct __attribute__((__packed__)) {
	uint32_t freq_desviation; 		// 2MHz = 6710809
	uint8_t mod_pin;							// 7 = input
	uint8_t fsk_trigger_src;      // 1 = trigger A
	uint8_t trigger_src_def;			// 3 = MOD rising edge
}pll_fsk_config_t;

/** @brief Type of PLL state. */
typedef enum {
	PLL_DISABLE = 0,
	PLL_ENABLE = 1
}pll_state_t;

/* Functions */

/**
 * @brief Initializes PLL configuration for TX radar frequencies.
 *
 * @param config Will contain configuration parameters.
 * @retval none
 */
void set_config_pll(pll_fsk_config_t *config);

/**
 * @brief Set PLL state for start/stop PLL frequencies generation.
 *
 * @param state Will contain sate for start/stop PLL module.
 * @retval none
 */
void set_pll_state(pll_state_t state);

/**
 * @brief Set enable DAC module for start transmission.
 *
 * @param none.
 * @retval none
 */
void start_fsk_transmission(void);

/**
  * @brief  Function implementing stop FSK signal transmission.
  * @param  None
  * @retval None
  */
void stop_fsk_transmission(void);

/**
 * @brief Callback for DAC_DMA .
 *
 * @param hdma Will contain DMA instance.
 * @retval none
 */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdma);

/* Variables */


#endif /* FSK_GENERATOR_H_ */
