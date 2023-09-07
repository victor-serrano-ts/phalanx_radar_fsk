/*
 * fsk_generator.c
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#include "fsk_generator.h"
#include "main.h"
#include <stdint.h>


uint32_t data_sin[]  = {
		602, 630, 658, 686, 713, 741, 767, 794, 819, 845, 869, 893, 916, 938, 960, 3852, 3871, 3890, 3907, 3923, 3938, 3952, 3965, 3976, 3987, 3995, 4003, 4009, 4014, 4017, 4020, 1148, 1148, 1146, 1142, 1137, 1131, 1124, 1115, 1105, 1093, 1080, 1067, 1051, 1035, 1018, 999, 3852, 3832, 3810, 3788, 3765, 3741, 3717, 3691, 3666, 3639, 3613, 3585, 3558, 3530, 3502, 3474, 574, 546, 518, 490, 462, 434, 407, 380, 354, 328, 303, 279, 255, 232, 209, 188, 3040, 3020, 3002, 2984, 2968, 2953, 2939, 2927, 2915, 2905, 2896, 2889, 2882, 2878, 2874, 2872, 0, 0, 2, 6, 11, 17, 24, 33, 43, 55, 67, 81, 96, 113, 130, 148, 3040, 3060, 3081, 3104, 3127, 3151, 3175, 3200, 3226, 3252, 3279, 3306, 3334, 3362, 3390, 3418, 574, 602, 630, 658, 686, 713, 741, 767, 794, 819, 845, 869, 893, 916, 938, 960, 3852, 3871, 3890, 3907, 3923, 3938, 3952, 3965, 3976, 3987, 3995, 4003, 4009, 4014, 4017, 4020, 1148, 1148, 1146, 1142, 1137, 1131, 1124, 1115, 1105, 1093, 1080, 1067, 1051, 1035, 1018, 999, 3852, 3832, 3810, 3788, 3765, 3741, 3717, 3691, 3666, 3639, 3613, 3585, 3558, 3530, 3502, 3474, 574, 546, 518, 490, 462, 434, 407, 380, 354, 328, 303, 279, 255, 232, 209, 188, 3040, 3020, 3002, 2984, 2968, 2953, 2939, 2927, 2915, 2905, 2896, 2889, 2882, 2878, 2874, 2872, 0, 0, 2, 6, 11, 17, 24, 33, 43, 55, 67, 81, 96, 113, 130, 148, 3040, 3060, 3081, 3104, 3127, 3151, 3175, 3200, 3226, 3252, 3279, 3306, 3334, 3362, 3390, 3418, 574,
};


void set_config_pll(pll_fsk_config_t *config)
{
	//Send config to PLL over microWire protocol

}

void set_pll_state(pll_state_t state)
{
	// Send state to PLL over microWire protocol

}


void start_fsk_transmission(void)
{
	pll_fsk_config_t pll_config;

	pll_config.freq_desviation = 0x666619; 		// 6710809 = 0x666619 = 2MHz
	pll_config.mod_pin = 0x7;									// 7 = input
	pll_config.fsk_trigger_src = 0x1; 				// 1 = trigger A
	pll_config.trigger_src_def = 0x3;					// 3 = MOD rising edge

//	setConfigPll(&pll_config);

//	setPllState(PLL_ENABLE);

	if (HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, &data_sin[0], sizeof(data_sin)/sizeof(data_sin[0]), DAC_ALIGN_12B_R) != HAL_OK)
	{
		/* Start DMA Error */
		Error_Handler();
	}
}

void stop_fsk_transmission(void)
{
	if (HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1) != HAL_OK)
	{
		/* Stop DMA Error */
		Error_Handler();
	}

//	setPllState(PLL_DISABLE);

//	/* For future PLL control */
//  if(HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_3) != HAL_OK)
//  {
//	  Error_Handler();
//  }
}


void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdma)
{
  /* Add code to be performed after DMA half complete */
	asm("NOP");
}

