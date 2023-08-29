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
#define LENGTH_SAMPLES 					2048
#define FFT_SIZE 								1024

/* Functions */
void initFftModule(void);
void getRx1Parameters(void);
uint32_t getRx1frequency(void);

void getRx2Parameters(void);
uint32_t getRx2frequency(void);

/* Variables */

extern uint32_t rx1_freq;
extern uint32_t rx2_freq;
extern uint32_t total_ffts;

#endif /* FSK_PROCESSING_H_ */
