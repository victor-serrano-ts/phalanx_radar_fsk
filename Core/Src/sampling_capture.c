/*
 * sampling_capture.c
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#include "sampling_capture.h"
#include "app_threadx.h"
#ifdef DEBUG_RX_SIGNALS
	#include "rx_signals_test.h"
#endif

uint32_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE] = {0};

float32_t fsk[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL] = {0};

float32_t rx1_f1_cmplx[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float32_t rx1_f2_cmplx[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float32_t rx2_f1_cmplx[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float32_t rx2_f2_cmplx[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};

bool first_half_data_ready = false;
bool first_half_fft_done = false;
bool second_half_data_ready = false;
bool second_half_fft_done = false;
uint32_t adc_full_count = 0;
uint32_t adc_half_count = 0;

uint32_t adc_callbacks_count = 0;


void start_fsk_sampling_capture(void)
{
  if (HAL_ADC_Start_DMA(&hadc4, (uint32_t *)aADCxConvertedData, (ADC_CONVERTED_DATA_BUFFER_SIZE)) != HAL_OK)
  {
    Error_Handler();
  }
}

void stop_fsk_sampling_capture(void)
{
  if (HAL_ADC_Stop_DMA(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }
}


void fill_and_interpolate_fsk_rx_buffers(void)
{
	uint16_t f1_index = 0;
	uint16_t f2_index = 0;
	uint16_t last_f1_index = 0;
	uint16_t last_f2_index = 0;
	uint32_t step_offset = first_half_data_ready ? 0 : ADC_CONVERTED_DATA_BUFFER_STEP_OFFSET;

	// Clean RX buffers before filling them again.
	memset(rx1_f1_cmplx, 0, sizeof(rx1_f1_cmplx));
	memset(rx1_f2_cmplx, 0, sizeof(rx1_f2_cmplx));
	memset(rx2_f1_cmplx, 0, sizeof(rx2_f1_cmplx));
	memset(rx2_f2_cmplx, 0, sizeof(rx2_f2_cmplx));

	if ( (first_half_data_ready && !first_half_fft_done) || (second_half_data_ready && !second_half_fft_done) ) {
		for (int i = 0; i < ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL; i++) {

			fsk[i] = (float32_t) aADCxConvertedData[ADC_FSK_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

			if (fsk[i] < FSK_THRESHOLD && f1_index < (ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2)) {
				rx1_f1_cmplx[2 * i] = (float32_t) aADCxConvertedData[ADC_RX1_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx1_f1_cmplx[2 * i + 1] = (float32_t) aADCxConvertedData[ADC_RX1_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f1_cmplx[2 * i] = (float32_t) aADCxConvertedData[ADC_RX2_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f1_cmplx[2 * i + 1] = (float32_t) aADCxConvertedData[ADC_RX2_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

				//TODO CMSIS DSP interpolation
				if ((i - last_f1_index) > 1) {
					float32_t rx1_f1_i_linear_slope = ((rx1_f1_cmplx[2 * i] - rx1_f1_cmplx[2 * last_f1_index])/(i - last_f1_index));
					float32_t rx1_f1_q_linear_slope = ((rx1_f1_cmplx[2 * i + 1] - rx1_f1_cmplx[2 * last_f1_index + 1])/(i - last_f1_index));
					float32_t rx2_f1_i_linear_slope = ((rx2_f1_cmplx[2 * i] - rx2_f1_cmplx[2 * last_f1_index])/(i - last_f1_index));
					float32_t rx2_f1_q_linear_slope = ((rx2_f1_cmplx[2 * i + 1] - rx2_f1_cmplx[2 * last_f1_index + 1])/(i - last_f1_index));

					for (int j = last_f1_index + 1; j < i; j++) {
						rx1_f1_cmplx[2 * j] = rx1_f1_cmplx[2 * last_f1_index] + rx1_f1_i_linear_slope * (j - last_f1_index);
						rx1_f1_cmplx[2 * j + 1] = rx1_f1_cmplx[2 * last_f1_index + 1] + rx1_f1_q_linear_slope * (j - last_f1_index);
						rx2_f1_cmplx[2 * j] = rx2_f1_cmplx[2 * last_f1_index] + rx2_f1_i_linear_slope * (j - last_f1_index);
						rx2_f1_cmplx[2 * j + 1] = rx2_f1_cmplx[2 * last_f1_index + 1] + rx2_f1_q_linear_slope * (j - last_f1_index);
					}
				}
				last_f1_index = i;

			} else if (f2_index < (ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2)){
				rx1_f2_cmplx[2 * i] = (float32_t) aADCxConvertedData[ADC_RX1_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx1_f2_cmplx[2 * i + 1] = (float32_t) aADCxConvertedData[ADC_RX1_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f2_cmplx[2 * i] = (float32_t) aADCxConvertedData[ADC_RX2_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f2_cmplx[2 * i + 1] = (float32_t) aADCxConvertedData[ADC_RX2_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

				//TODO CMSIS DSP interpolation
				if ((i - last_f2_index) > 1) {
					float32_t rx1_f2_i_linear_slope = ((rx1_f2_cmplx[2 * i] - rx1_f2_cmplx[2 * last_f2_index])/(i - last_f2_index));
					float32_t rx1_f2_q_linear_slope = ((rx1_f2_cmplx[2 * i + 1] - rx1_f2_cmplx[2 * last_f2_index + 1])/(i - last_f2_index));
					float32_t rx2_f2_i_linear_slope = ((rx2_f2_cmplx[2 * i] - rx2_f2_cmplx[2 * last_f2_index])/(i - last_f2_index));
					float32_t rx2_f2_q_linear_slope = ((rx2_f2_cmplx[2 * i + 1] - rx2_f2_cmplx[2 * last_f2_index + 1])/(i - last_f2_index));

					for (int j = last_f2_index + 1; j < i; j++) {
						rx1_f2_cmplx[2 * j] = rx1_f2_cmplx[2 * last_f2_index] + rx1_f2_i_linear_slope * (j - last_f2_index);
						rx1_f2_cmplx[2 * j + 1] = rx1_f2_cmplx[2 * last_f2_index + 1] + rx1_f2_q_linear_slope * (j - last_f2_index);
						rx2_f2_cmplx[2 * j] = rx2_f2_cmplx[2 * last_f2_index] + rx2_f2_i_linear_slope * (j - last_f2_index);
						rx2_f2_cmplx[2 * j + 1] = rx2_f2_cmplx[2 * last_f2_index + 1] + rx2_f2_q_linear_slope * (j - last_f2_index);
					}
				}

				last_f2_index = i;

			}
		}
	}

	/* interpolation test */
	//	float32_t interpolation_test_reference[20] = {0.0,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5};
	//	float32_t interpolation_test_in[10] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0};
	//	float32_t interpolation_test_out[20] = {0};
	//	//#define XSPACING               (0.005f)
	//	float32_t xspacing = 0.01;
	//
	//	arm_linear_interp_instance_f32 interpolation_instance = {10, 1.0, xspacing/*XSPACING*/, (float*)&interpolation_test_reference[0]};
	//
	//	for(int i = 0 ; i < 20 ; i+=2) {
	//		interpolation_test_out[i] = arm_linear_interp_f32(&interpolation_instance, interpolation_test_in[i]);
	//		//interpolation_instance.pYData =
	//	}

	//arm_spline_instance_f32 interpolation_instance = ();

}

void fill_rx_buffers_test (void)
{
	int j = 0;

	for (int i = 0; i < ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL; i++) {
		// Rx1_f1
		rx1_f1_cmplx[j] = rx1f1_i[i];
		rx1_f1_cmplx[j+1] = rx1f1_q[i];
		// Rx1_f2
		rx1_f2_cmplx[j] = rx1f2_i[i];
		rx1_f2_cmplx[j+1] = rx1f2_q[i];
		// Rx2_f1
		rx2_f1_cmplx[j] = rx2f1_i[i];
		rx2_f1_cmplx[j+1] = rx2f1_q[i];

		j+=2;
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Add code to be performed after DMA half complete */
  adc_half_count++;
  adc_callbacks_count++;
  first_half_data_ready = true;
  first_half_fft_done = false;
  second_half_data_ready = false;
  second_half_fft_done = false;
  if (tx_event_flags_set(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR) != TX_SUCCESS)
	{
		Error_Handler();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Add code to be performed after DMA full complete */
	adc_full_count++;
	adc_callbacks_count++;
	first_half_data_ready = false;
	first_half_fft_done = false;
	second_half_data_ready = true;
	second_half_fft_done = false;
  if (tx_event_flags_set(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR) != TX_SUCCESS)
	{
		Error_Handler();
	}
}

