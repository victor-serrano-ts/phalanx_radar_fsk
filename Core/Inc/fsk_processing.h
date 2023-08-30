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
//#include "arm_math.h"

/* Constants */
#define LENGTH_SAMPLES 					2048
#define FFT_SIZE 								1024

/* Types */
typedef struct {
	float32_t bin_level;
	float32_t frequency_hz;
	float32_t angle;
	float32_t speed_kmh;
	float32_t distance;
	uint32_t motion;
	float32_t noise;
	int32_t acc_max;
} fsk_result_t;

/* Functions */
void initFftModule(void);

fsk_result_t getDetectionParameters(void);
float32_t getBinLevel(void);
float32_t getFrequency(float32_t * rx_data_samples);
float32_t getAngle(void);
float32_t getSpeed(void);
float32_t getDistance(void);
uint32_t 	getMotion(void);
float32_t getNoise(void);
int32_t 	getAccMax(void);

/*
void getRx2parameters(void);
float32_t getRx1binLevel(void);
float32_t getRx1frequency(void);
float32_t getRx1angle(void);
float32_t getRx1speed(void);
float32_t getRx1motion(void);
float32_t getRx1noise(void);
*/

/*
void getRx2parameters(void);
float32_t getRx2binLevel(void);
float32_t getRx2frequency(void);
float32_t getRx2angle(void);
float32_t getRx2speed(void);
float32_t getRx2motion(void);
float32_t getRx2noise(void);
*/

/* Variables */
extern uint32_t rx1_freq;
extern uint32_t rx2_freq;
extern uint32_t total_ffts;

#endif /* FSK_PROCESSING_H_ */
