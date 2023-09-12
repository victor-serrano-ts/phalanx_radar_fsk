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

/**
 * @brief Compute the OS-CFAR of the data.
 *
 * @param[in] buffer_cell Input data cells.
 * @param[in] test_cell Location of the cell to which the CFAR will be applied.
 * @param[in] active_cells Number of active cells to use on each side of the
 * target.
 * @param[in] guard_cells Number of guard cells to use on each side of the
 * target.
 *
 * @return Estimated noise level by the CFAR algorithm.
 */
static float fft_os_cfar(float *buffer_cell, size_t test_cell,
                         size_t active_cells, size_t guard_cells)
{
    // Compute the lagging and leading indexes
    int idx_lagging_start = (int)test_cell - active_cells - guard_cells;
    int idx_lagging_end = (int)test_cell - guard_cells;
    int idx_leading_start = (int)test_cell + 1 + guard_cells;
    int idx_leading_end = (int)test_cell + 1 + active_cells + guard_cells;

    if (idx_lagging_start < 0)
    {
        idx_lagging_start = FSK_FFT_SIZE + idx_lagging_start;
    }

    if (idx_lagging_end < 0)
    {
        idx_lagging_end = /*fft_len*/FSK_FFT_SIZE + idx_lagging_end;
    }

    if (idx_leading_start >= FSK_FFT_SIZE)
    {
        idx_leading_start = idx_leading_start - FSK_FFT_SIZE;
    }

    if (idx_leading_end >= FSK_FFT_SIZE)
    {
        idx_leading_end = idx_leading_end - FSK_FFT_SIZE;
    }

    // Copy the active cells in the second half of the buffer (not used)
    int idx_cfar = FSK_FFT_SIZE;

    if (idx_lagging_start > idx_lagging_end)
    {
        for (int i = idx_lagging_start; i < FSK_FFT_SIZE; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }

        for (int i = 0; i < idx_lagging_end; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }
    }
    else
    {
        for (int i = idx_lagging_start; i < idx_lagging_end; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }
    }

    if (idx_leading_start > idx_leading_end)
    {
        for (int i = idx_leading_start; i < FSK_FFT_SIZE; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }

        for (int i = 0; i < idx_leading_end; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }
    }
    else
    {
        for (int i = idx_leading_start; i < idx_leading_end; i++)
        {
            buffer_cell[idx_cfar++] = buffer_cell[i];
        }
    }

    // Sort the active cells in ascending order
    const arm_sort_instance_f32 sort_instance =
    {
        .alg = ARM_SORT_QUICK,
        .dir = ARM_SORT_ASCENDING,
    };

    arm_sort_f32(&sort_instance, &buffer_cell[FSK_FFT_SIZE], &buffer_cell[FSK_FFT_SIZE],
                 active_cells * 2);

    // Select the 3/4 th element as a noise estimation
    idx_cfar = FSK_FFT_SIZE + active_cells * 2 * 3 / 4 - 1;

    return buffer_cell[idx_cfar];
}


void fft_estimate_noise(float32_t *complex_data_buf, float32_t *noise_level, uint32_t target_bin)
{
//  if (fft_active_cells > 0)
//  {
		// The noise is estimated using a CFAR algorithm
		*noise_level = fft_os_cfar(complex_data_buf, target_bin, DOPPLER_NOISE_ACTIVE_CELLS,
				DOPPLER_NOISE_GUARD_CELLS);

		// Get the amplitude in quantity peak (sqrt(fft_r^2 + fft_i^2)/signal_len)
		// https://www.sjsu.edu/people/burford.furman/docs/me120/FFT_tutorial_NI.pdf
		arm_sqrt_f32(*noise_level, noise_level);
		*noise_level /= (float32_t)/*fft_window_len*/FSK_FFT_SIZE;
//  }
//  else
//  {
//		*noise_level = 0.0f;
//  }
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
	fft_estimate_noise(rx1_f1_cmplx,
										 &detection_results.noise_level,
										 (uint32_t)detection_results.bin_level);

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
	float32_t rx1f1_noise = 0.0;
	float32_t rx1f2_noise = 0.0;
	float32_t rx2f1_noise = 0.0;



	/* rx1_f1 signal processing */
	arm_cfft_f32(&varInstCfftF32, rx1_f1, ifftFlag, doBitReverse);
	arm_cmplx_mag_squared_f32(rx1_f1, testOutput, FSK_FFT_SIZE);
	testOutput[0] = 0;
	arm_max_f32(testOutput, FSK_FFT_SIZE, &rx1f1_max_bin, &rx1f1_maxIndex);
	rx1f1_freq = SAMPLE_FREQ * rx1f1_maxIndex / FSK_FFT_SIZE;
	rx1f1_phase = get_phase(rx1_f1[rx1f1_maxIndex*2], rx1_f1[rx1f1_maxIndex*2+1]);

	fft_estimate_noise(rx1_f1, &rx1f1_noise, rx1f1_max_bin);

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
