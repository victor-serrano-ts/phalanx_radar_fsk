/*
 * fsk_generator.c
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#include "fsk_generator.h"
#include "main.h"
#include <stdint.h>


uint32_t data_sin[32]  = {
		4020, 4020, 4020, 4020, 4020, 4020, 4020, 4020,
		4020, 4020, 4020, 4020, 4020, 4020, 4020, 4020,
		0,    0,    0,    0,    0,    0,    0,    0,
		0,    0,    0,    0,    0,    0,    0,    0,
};
/*uint32_t data_sin[128]  = {
  2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897,
  2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640,
  3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002,
  4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894,
  3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344,
  3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483,
  2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517,
  1424, 1332, 1242, 1154, 1068, 985,  904,  826,  751,  679,
  610,  545,  484,  426,  373,  323,  278,  237,  201,  169,
  141,  118,  100,  87,   79,   75,   76,   82,   93,   109,
  129,  154,  184,  218,  257,  300,  347,  399,  455,  514,
  577,  644,  714,  788,  865,  944,  1026, 1111, 1198, 1287,
  1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047
};*/


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

