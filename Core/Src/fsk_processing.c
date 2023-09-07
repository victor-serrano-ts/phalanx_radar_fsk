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
static float32_t testOutput[FSK_LENGTH_SAMPLES/2];

uint32_t total_ffts = 0;
uint32_t rx1_freq = 0;
uint32_t rx2_freq = 0;



void init_fft_module(void)
{
	arm_cfft_init_f32(&varInstCfftF32,FSK_FFT_SIZE);
}

fsk_result_t get_detection_parameters(void)
{
	fsk_result_t detection_results;

	fsk_process(&detection_results, &rx1_f1_cmplx[0]);

	return detection_results;
}

void fsk_process(fsk_result_t *result, float32_t *signal_data)
{
	float32_t max_bin = 0.0;
	uint32_t maxIndex = 0;

	//float32_t snr1 = 0.0;

	arm_cfft_f32(&varInstCfftF32, signal_data, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(signal_data, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &max_bin, &maxIndex);

	result->frequency_hz = SAMPLE_FREQ * maxIndex / FSK_FFT_SIZE;

	result->bin_level = max_bin;

	if(result->bin_level < (FSK_FFT_SIZE / 2)) {
		result->motion = MOTION_DEPARTING;
	} else {
		result->motion = MOTION_APPROACHING;
		result->bin_level -= FSK_FFT_SIZE;
	}

	//result->angle = ; // TODO - Con el desfase de f1 de Rx1 y Rx2 se calcula el ángulo
	//result->distance = ; //// TODO - Con la diferencia de fase de f1 y f2 de Rx1
	result->speed_kmh = result->frequency_hz * FSK_HZ_TO_KMH;
	//result->noise_level = ;  // // TODO - No encuentro función del CMSIS que lo haga directo
}
