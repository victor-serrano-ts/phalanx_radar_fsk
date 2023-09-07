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
	fsk_result_t detection_results = {0};

	/* frequency, bin level, angle and distance calculation */
	fsk_process(&detection_results,
							&rx1_f1_cmplx[0],
							&rx1_f2_cmplx[0],
							&rx2_f1_cmplx[0]);

	/* motion calculation and frequency correction*/
	if(detection_results.frequency_hz <= (FSK_FFT_SIZE / 2)) {
		detection_results.motion = MOTION_DEPARTING;
	} else {
		detection_results.motion = MOTION_APPROACHING;
		detection_results.frequency_hz = FSK_FFT_SIZE - detection_results.frequency_hz;
	}

	/* speed calculation */
	detection_results.speed_kmh = detection_results.frequency_hz * FSK_HZ_TO_KMH; // TODO - comprobar fórmula y adapatar

	/* noise level calculation */
	//result->noise_level = ;  // // TODO - implementar CFAR y hacer lo de RIDE



	return detection_results;
}

void fsk_process(fsk_result_t *result, float32_t *rx1_f1, float32_t *rx1_f2, float32_t *rx2_f1)
{
	float32_t rx1f1_max_bin = 0.0;
	float32_t rx1f2_max_bin = 0.0;
	float32_t rx2f1_max_bin = 0.0;
	uint32_t rx1f1_maxIndex = 0;
	uint32_t rx1f2_maxIndex = 0;
	uint32_t rx2f1_maxIndex = 0;
	float32_t rx1f1_phase = 0.0;
	float32_t rx1f2_phase = 0.0;
	float32_t rx2f1_phase = 0.0;
	uint32_t rx1f1_freq = 0;
	uint32_t rx1f2_freq = 0;
	uint32_t rx2f1_freq = 0;

	/* rx1_f1 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx1_f1, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx1_f1, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx1f1_max_bin, &rx1f1_maxIndex);
	rx1f1_freq = SAMPLE_FREQ * rx1f1_maxIndex / FSK_FFT_SIZE;
	rx1f1_phase = atan2(rx1_f1[rx1f1_maxIndex*2+1], rx1_f1[rx1f1_maxIndex*2]);
	total_ffts++;

	/* rx2_f1 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx2_f1, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx2_f1, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx2f1_max_bin, &rx2f1_maxIndex);
	rx2f1_freq = SAMPLE_FREQ * rx2f1_maxIndex / FSK_FFT_SIZE;
	rx2f1_phase = atan2(rx2_f1[rx2f1_maxIndex*2+1], rx2_f1[rx2f1_maxIndex*2]);
	total_ffts++;

	/* rx1_f2 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx1_f2, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx1_f2, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx2f1_max_bin, &rx1f2_maxIndex);
	rx1f2_freq = SAMPLE_FREQ * rx1f2_maxIndex / FSK_FFT_SIZE;
	rx1f2_phase = atan2(rx1_f2[rx1f2_maxIndex*2+1], rx1_f2[rx1f2_maxIndex*2]);
	total_ffts++;

	/* bin level calculation */
	result->bin_level = rx1f1_max_bin;

	/* frequency calculation */
	result->frequency_hz = SAMPLE_FREQ * rx1f1_maxIndex / FSK_FFT_SIZE;

	/* Angle calculation */
	result->angle = rx1f1_phase - rx2f1_phase; // TODO - Con el fase de f1 de Rx1 y Rx2 se calcula el ángulo y la distancia entre antenas

	/* distance calculation */
	//result->distance = ; // TODO - Con la diferencia de fase de f1 y f2 de Rx1, diferencia de frecuencias y la velocidad de la luz
}
