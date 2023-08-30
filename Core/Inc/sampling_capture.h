/*
 * sampling_capture.h
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#ifndef SAMPLING_CAPTURE_H_
#define SAMPLING_CAPTURE_H_

/*Includes*/
#include "main.h"

/* FSK Constants */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  10240)   /* Size of array aADCxConvertedData[] */
#define ADC_CONVERTED_DATA_BUFFER_STEPS 2
#define ADC_CONVERTED_DATA_BUFFER_STEP_OFFSET (ADC_CONVERTED_DATA_BUFFER_SIZE/2)
#define ADC_ENABLED_CHANNEL_COUNT ((uint32_t) 5)
#define ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL (ADC_CONVERTED_DATA_BUFFER_SIZE/ADC_ENABLED_CHANNEL_COUNT/ADC_CONVERTED_DATA_BUFFER_STEPS)
#define FSK_THRESHOLD ((float) 512.0)
#define ADC_RX1_I_OFFSET 0
#define ADC_RX1_Q_OFFSET 1
#define ADC_RX2_I_OFFSET 2
#define ADC_RX2_Q_OFFSET 3
#define ADC_FSK_OFFSET   4

/* FSK Functions */
void startFskSamplingCapture(void);
void stopFskSamplingCapture(void);
void fillAndInterpolateFskAndRxBuffers(void);

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

/* FSK Variables */
extern uint32_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
extern float fsk[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL];
extern float rx1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];
extern float rx2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];
extern float rx1_f1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];
extern float rx1_f2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];
extern float rx2_f1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];
extern float rx2_f2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2];

extern bool first_half_data_ready;
extern bool first_half_fft_done;
extern bool second_half_data_ready;
extern bool second_half_fft_done;
extern uint32_t adc_full_count;
extern uint32_t adc_half_count;

/* FMCW Constants */
/* FMCW Functions */
/* FMCW Variables */

#endif /* SAMPLING_CAPTURE_H_ */
