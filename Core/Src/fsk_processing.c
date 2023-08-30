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
  * @brief  Function implementing get bin, frequency, angle, speed, distance, motion and noise parameters for RX1
  * @param  None
  * @retval None
  */
fsk_result_t getDetectionParameters(void)
{
	fsk_result_t detection_results;

rx1_freq = getFrequency(&rx1[0]); // Delete, only test!!!
rx2_freq = getFrequency(&rx2[0]); // Delete, only test !!!

	//Calculate bin level
	detection_results.bin_level = getBinLevel();
	//Calculate frequency
	//detection_results.frequency_hz = getFrequency(&rx1[0);
	//Calculate angle
	detection_results.angle = getAngle();
	//Calculate speed
	detection_results.speed_kmh = getSpeed();
	//Calculate distance
	detection_results.distance = getDistance();
	//Calculate motion
	detection_results.motion = getMotion();
	//Calculate noise
	detection_results.noise = getNoise();
	//Get acceleration parameter
	detection_results.acc_max = getAccMax();

	return detection_results;
}

/**
  * @brief  Function implementing get bin level detected
  * @param  None
  * @retval float32_t
  */
float32_t getBinLevel(void)
{
	float32_t bin_level_t = 0.0;


	return bin_level_t;
}

/**
  * @brief  Function implementing get frequency detected
  * @param  None
  * @retval float
  */
float32_t getFrequency(float32_t * rx_data_samples)
{
  float32_t rx_maxValue = 0.0;
  uint32_t rx_maxIndex = 0;
	float32_t frequency_t = 0.0;


	arm_cfft_f32(&varInstCfftF32, rx_data_samples, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx_data_samples, testOutput, FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FFT_SIZE, &rx_maxValue, &rx_maxIndex);
	frequency_t = SAMPLE_FREQ * rx_maxIndex / FFT_SIZE;
	total_ffts++;

	return frequency_t;
}

/**
  * @brief  Function implementing get angle detected
  * @param  None
  * @retval float32_t
  */
float32_t getAngle(void)
{
	float32_t angle_t = 0.0;



	return angle_t;
}

/**
  * @brief  Function implementing get speed detected
  * @param  None
  * @retval float32_t
  */
float32_t getSpeed(void)
{
	float32_t speed_t = 0.0;



	return speed_t;
}

/**
  * @brief  Function implementing get distance detected (cm or m?)
  * @param  None
  * @retval float32_t
  */
float32_t getDistance(void)
{
	float32_t distance_t = 0.0;



	return distance_t;
}

/**
  * @brief  Function implementing get motion detected
  * @param  None
  * @retval uint32_t
  */
uint32_t getMotion(void)
{
	uint32_t motion_t = 0;



	return motion_t;
}

/**
  * @brief  Function implementing get noise detected
  * @param  None
  * @retval float32_t
  */
float32_t getNoise(void)
{
	float32_t noise_t = 0.0;



	return noise_t;
}

int32_t getAccMax(void)
{
	int32_t acc_max_t = 0;


	return acc_max_t;
}

///**
//  * @brief  Function implementing get bin, frequency, angle, speed, distance, motion and noise parameters for RX2
//  * @param  None
//  * @retval None
//  */
//void getRx2Parameters(void)
//{
//	singal_processing_result_t rx2_results_t;
//
//
//rx2_freq = getRx1frequency(); // Delete, only test !!!
//
//	//Rx2 bin level
//	rx2_results_t.bin_level = getRx2binLevel();
//	//Rx2 frequency
//	rx2_results_t.frequency = getRx2frequency();
//	//Rx2 angle
//	rx2_results_t.angle = getRx2angle();
//	//Rx2 speed
//	rx2_results_t.speed = getRx2speed();
//	//Rx2 distance
//	rx2_results_t.distance = getRx2distance();
//	//Rx2 motion
//	rx2_results.motion = getRx2motion();
//	//Rx2 noise
//	rx2_results.noise = getRx2noise();
//}
//
///**
//  * @brief  Function implementing analyze max frequency from RX1 samples
//  * @param  None
//  * @retval None
//  */
//uint32_t getRx2frequency(void)
//{
//	uint32_t rx2_freq_t = 0;
//
//
//	arm_cfft_f32(&varInstCfftF32, rx2, ifftFlag, doBitReverse);
//	arm_cmplx_mag_squared_f32(rx2, testOutput, FFT_SIZE);
//	testOutput[0] = 0;
//	arm_max_f32(testOutput, FFT_SIZE, &rx2_maxValue, &rx2_maxIndex);
//	rx2_freq_t = SAMPLE_FREQ * rx2_maxIndex / FFT_SIZE;
//	total_ffts++;
//
//	return rx2_freq_t;
//}
