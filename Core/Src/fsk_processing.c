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
	if(detection_results.frequency_hz <= (SAMPLE_FREQ / 2)) {
		detection_results.motion = MOTION_DEPARTING;
	} else {
		detection_results.motion = MOTION_APPROACHING;
		detection_results.frequency_hz = SAMPLE_FREQ - detection_results.frequency_hz;
	}

	/* speed calculation */
	detection_results.speed_kmh = detection_results.frequency_hz * FSK_HZ_TO_KMH;
	detection_results.speed_kmh = detection_results.speed_kmh / cos((detection_results.angle * PI) / 180.0);

	/* noise level calculation */
	//result->noise_level = ;  // // TODO - Implement CFAR like RIDE project

	return detection_results;
}

float32_t  get_phase(float32_t real, float32_t imag)
{
	float32_t phi = 0.0;

	if((real > 0) && (imag >= 0)) {								//1st quadrant
		phi = atan((float32_t)imag / (float32_t)real);
	}	else if((real < 0) && (imag >= 0)) {				// 2nd quadrant
		phi = atan((float32_t)imag / (float32_t)real) + PI;
	} else if ((real < 0) && (imag <= 0)) {				// 3rd quadrant
		phi = atan((float32_t)imag / (float32_t)real) + PI;
	}	else if ((real > 0) && (imag <= 0)) {				// 4th quadrant
		phi = atan((float32_t)imag / (float32_t)real) + 2*PI;
	}	else if ((real == 0) && (imag > 0)) {
		phi = PI/2;
	} else if ((real == 0) && (imag < 0)) {
		phi = 3*PI/2;
	} else {
		phi = 0;
	}

	return phi;
}

void fsk_process(fsk_result_t *result, float32_t *rx1_f1, float32_t *rx1_f2, float32_t *rx2_f1)
{
	float32_t rx1f1_max_bin = 0.0;
	float32_t rx1f2_max_bin = 0.0;
	float32_t rx2f1_max_bin = 0.0;
	uint32_t rx1f1_maxIndex = 0;
	uint32_t rx1f2_maxIndex = 0;
	uint32_t rx2f1_maxIndex = 0;

	float32_t phase_diff = 0.0;
	float32_t target_angle = 0.0;
	float32_t delta_angle = 0.0;

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
	rx1f1_phase = get_phase(rx1_f1[rx1f1_maxIndex*2], rx1_f1[rx1f1_maxIndex*2+1]);

	total_ffts++;

	/* rx2_f1 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx2_f1, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx2_f1, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx2f1_max_bin, &rx2f1_maxIndex);
	rx2f1_freq = SAMPLE_FREQ * rx2f1_maxIndex / FSK_FFT_SIZE;
	rx2f1_phase = get_phase(rx2_f1[rx2f1_maxIndex*2], rx2_f1[rx2f1_maxIndex*2+1]);

	total_ffts++;

	/* rx1_f2 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx1_f2, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx1_f2, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx1f2_max_bin, &rx1f2_maxIndex);
	rx1f2_freq = SAMPLE_FREQ * rx1f2_maxIndex / FSK_FFT_SIZE;
	rx1f2_phase = get_phase(rx1_f2[rx1f2_maxIndex*2], rx1_f2[rx1f2_maxIndex*2+1]);
	total_ffts++;

	/* bin level calculation */
	result->bin_level = rx1f1_max_bin;

	/* frequency calculation */
	//result->frequency_hz = SAMPLE_FREQ * rx1f1_maxIndex / FSK_FFT_SIZE;
	result->frequency_hz = SAMPLE_FREQ * rx1f1_maxIndex / FSK_FFT_SIZE;

	/* Angle calculation */
	phase_diff = (rx1f1_phase - rx2f1_phase);


	/* Arcus sinus (-PI/2 to PI/2), input= -1..1 */
	target_angle = asin(phase_diff * WAVE_LENGHT_ANT_SPACING_RATIO / (2*PI));

	target_angle = target_angle * 180 / PI;	// Angle (-90...90°)

	result->angle = target_angle;

	/* distance calculation */
	phase_diff = (rx1f1_phase - rx1f2_phase);

	if (phase_diff <= 0)
	{
		phase_diff += 2*PI;
	}

	result->distance = ((LIGHT_VELOCITY * phase_diff) / (4 * PI * FREQUENCY_DIFF));
}
