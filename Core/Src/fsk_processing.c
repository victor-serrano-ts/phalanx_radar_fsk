/*
 * fsk_processing.c
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#include "fsk_processing.h"
#include "sampling_capture.h"

static arm_cfft_instance_f32 varInstCfftF32;
static uint32_t ifftFlag = 0;
static uint32_t doBitReverse = 1;
static float32_t testOutput[LENGTH_SAMPLES/2];

uint32_t total_ffts = 0;

static float32_t rx1_maxValue = 0.0;
static uint32_t rx1_maxIndex = 0;
static float32_t rx2_maxValue = 0.0;
static uint32_t rx2_maxIndex = 0;

uint32_t rx1_freq = 0;
uint32_t rx2_freq = 0;

/**
  * @brief  Function implementing FFT module initialization
  * @param  None
  * @retval None
  */
void initFftModule(void)
{
	arm_cfft_init_f32(&varInstCfftF32,FFT_SIZE);
}

/**
  * @brief  Function implementing get bin, frequency, speed, angle and distance  parameters for RX1
  * @param  None
  * @retval None
  */
void getRx1Parameters(void)
{
	//Rx1 bin level
	//Rx1 frequency
	rx1_freq = getRx1frequency();
	//Rx1 angle
	//Rx1 speed
	//Rx1 distance
	//Rx1 direction
}

/**
  * @brief  Function implementing get bin level, frequency, speed, angle and distance parameters for RX1
  * @param  None
  * @retval uint32_t
  */
uint32_t getRx1frequency(void)
{
	uint32_t rx1_freq_t = 0;


	arm_cfft_f32(&varInstCfftF32, rx1, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx1, testOutput, FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FFT_SIZE, &rx1_maxValue, &rx1_maxIndex);
	rx1_freq_t = SAMPLE_FREQ * rx1_maxIndex / FFT_SIZE;
	total_ffts++;

	return rx1_freq_t;
}

/**
  * @brief  Function implementing get bin level, frequency, speed, angle and distance parameters for RX2
  * @param  None
  * @retval None
  */
void getRx2Parameters(void)
{
	//Rx2 bin level
	//Rx2 frequency
	rx2_freq = getRx2frequency();
	//Rx2 angle
	//Rx2 speed
	//Rx2 distance
	//Rx2 direction
}

/**
  * @brief  Function implementing analyze max frequency from RX1 samples
  * @param  None
  * @retval None
  */
uint32_t getRx2frequency(void)
{
	uint32_t rx2_freq_t = 0;


	arm_cfft_f32(&varInstCfftF32, rx2, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx2, testOutput, FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FFT_SIZE, &rx2_maxValue, &rx2_maxIndex);
	rx2_freq_t = SAMPLE_FREQ * rx2_maxIndex / FFT_SIZE;
	total_ffts++;

	return rx2_freq_t;
}
