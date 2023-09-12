/*
 * fsk_processing.h
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#ifndef FSK_PROCESSING_H_
#define FSK_PROCESSING_H_

/* Includes */
#include "arm_math.h"

/* Constants */

/** FFT length for Doppler mode. */
#define FSK_LENGTH_SAMPLES 								2048
#define FSK_FFT_SIZE 											1024

#define BSP_TGT_WAVE_LENGTH_MM						(12.4f)
#define BSP_TGT_ANTENNA_SPACING_MM				(6.22f)
#define WAVE_LENGHT_ANT_SPACING_RATIO			(BSP_TGT_WAVE_LENGTH_MM / BSP_TGT_ANTENNA_SPACING_MM)

#define LIGHT_VELOCITY										(300000000.0f)
#define FREQUENCY_DIFF										(2000000.0f)

/** Number of active cells to use on each side of the target bin to estimate the
 * noise. */
#define DOPPLER_NOISE_ACTIVE_CELLS				(20)

/** Number of guard cells to use on each side of the target bin to estimate the
 * noise. */
#define DOPPLER_NOISE_GUARD_CELLS         (5)

/** Frequency (Hz) to speed (km/h). Conversion:
 * (c /(2 * f_tx) = 3e8 / (2 * 24.125e9) / 1000 * 3600) */
#define FSK_HZ_TO_KMH                   	(1.0f / 44.4f)

/* Types */

/** @brief Result of fsk signals processing. */
typedef struct __attribute__((__packed__)) {
	float32_t bin_level;					// FSK FFT maximum bin level
	float32_t frequency_hz;				// FSK frequency (Hz)
	float32_t angle;							// FSK angle of target
	float32_t speed_kmh;					// speed of target
	float32_t distance;						// distance of target (m)
	uint32_t motion;							// FSK detetcted direction
	float32_t noise_level;							// Estimated noise level
} fsk_result_t;

/** @brief Types of motions that can be detected by fsk signals
 * processing. */
typedef enum {
  MOTION_NONE = 0,
  MOTION_DEPARTING,
  MOTION_APPROACHING,
}fsk_motion_t;

/* Variables */
extern uint32_t total_ffts;


/* Functions */

/**
  * @brief  Function implementing FFT module initialization
  * @param  None
  * @retval None
  */
void init_fft_module(void);
/**
  * @brief  Function implementing get bin, frequency, angle, speed, distance, motion and noise parameters for RX
  * @param  None
  * @retval None
  */
fsk_result_t get_detection_parameters(void);

//TODO - Change this comment with final implementation
/**
 * @brief Process I and Q signals to obtaining the speed and direction of a
 * detected object. Steps done:
 * 1. Apply the FFT window and scale to I and Q signals.
 * 2. Rearrange I and Q signals as a single complex signal.
 * 3. Perform a FFT conversion to the complex signal.
 * 3. Get the magnitude (spectrum) from the FFT result.
 * 4. Get the maximum bin to determine the speed and direction.
 * 5. Estimate the noise from the cells surrounding the maximum bin.
 *
 * @param result Will contain the maximum FFT bin level, its frequency, the
 * speed of the object and the type of motion detected. Also, the estimated
 * noise level will be included.
 * @param signal_i Signal I to be processed.
 * @param signal_q Signal Q to be processed.
 */
void fsk_process(fsk_result_t *result, float32_t *rx1_f1, float32_t *rx1_f2, float32_t *rx2_f1);


void fft_estimate_noise(float32_t *complex_data_buf, float32_t *noise_level, uint32_t target_bin);

#endif /* FSK_PROCESSING_H_ */
