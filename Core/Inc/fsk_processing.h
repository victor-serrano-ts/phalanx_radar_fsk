/*
 * fsk_processing.h
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#ifndef FSK_PROCESSING_H_
#define FSK_PROCESSING_H_

/* Includes */
#include "main.h"

/* Constants */

/** FFT length for Doppler mode. */
#define FSK_LENGTH_SAMPLES 					2048
#define FSK_FFT_SIZE 								1024

/* Types */

/** @brief Result of fsk signals processing. */
typedef struct __attribute__((__packed__)) {
	float32_t bin_level;					// FSK FFT maximum bin level
	float32_t frequency_hz;				// FSK frequency (Hz)
	float32_t angle;							// FSK angle of target
	float32_t speed_kmh;					// speed of target
	float32_t distance;						// distance of target (TODO cm/m?)
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
void fsk_process(fsk_result_t *result, const float32_t *signal_i, const float32_t *signal_q);

/**
  * @brief  Function implementing get bin level detected
  * @param  None
  * @retval float32_t
  */
//float32_t getBinLevel(void);

/**
  * @brief  Function implementing get frequency detected
  * @param  None
  * @retval float32_t
  */
float32_t get_frequency(float32_t *rx_data_samples);

/**
  * @brief  Function implementing get angle detected
  * @param  None
  * @retval float32_t
  */
//float32_t getAngle(void);
/**
  * @brief  Function implementing get speed detected
  * @param  None
  * @retval float32_t
  */
//float32_t getSpeed(void);
/**
  * @brief  Function implementing get distance detected (cm or m?)
  * @param  None
  * @retval float32_t
  */
//float32_t getDistance(void);
/**
  * @brief  Function implementing get motion detected
  * @param  None
  * @retval uint32_t
  */
//uint32_t 	getMotion(void);
/**
  * @brief  Function implementing get noise detected
  * @param  None
  * @retval float32_t
  */
//float32_t getNoise(void);
/**
  * @brief  Function implementing get acceleration value
  * @param  None
  * @retval int32_t
  */
//int32_t 	getAccMax(void);

/* Variables */
extern uint32_t rx1_freq;
extern uint32_t rx2_freq;
extern uint32_t total_ffts;

#endif /* FSK_PROCESSING_H_ */
