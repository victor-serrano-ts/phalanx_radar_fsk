/*
 * sampling_capture.c
 *
 *  Created on: Aug 22, 2023
 *      Author: victor
 */

#include "sampling_capture.h"
#include "app_threadx.h"

uint32_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE] = {0};

float fsk[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL] = {0};
float rx1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float rx2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float rx1_f1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float rx1_f2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float rx2_f1[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};
float rx2_f2[ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2] = {0};

bool first_half_data_ready = false;
bool first_half_fft_done = false;
bool second_half_data_ready = false;
bool second_half_fft_done = false;
uint32_t adc_full_count = 0;
uint32_t adc_half_count = 0;

/**
  * @brief  Function implementing start FSK signal sampling capture.
  * @param  None
  * @retval None
  */
void startFskSamplingCapture(void)
{
  if (HAL_ADC_Start_DMA(&hadc4,
                        (uint32_t *)aADCxConvertedData,
                        (ADC_CONVERTED_DATA_BUFFER_SIZE)
                       ) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief  Function implementing stop FSK signal sampling capture.
  * @param  None
  * @retval None
  */
void stopFskSamplingCapture(void)
{
  if (HAL_ADC_Stop_DMA(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Conversion complete callback in non-blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Add code to be performed after DMA half complete */
  adc_half_count++;
  first_half_data_ready = true;
  first_half_fft_done = false;
  second_half_data_ready = false;
  second_half_fft_done = false;
  if (tx_event_flags_set(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR) != TX_SUCCESS)
	{
		Error_Handler();
	}

}

/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Add code to be performed after DMA full complete */
	adc_full_count++;
	first_half_data_ready = false;
	first_half_fft_done = false;
	second_half_data_ready = true;
	second_half_fft_done = false;
  if (tx_event_flags_set(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR) != TX_SUCCESS)
	{
		Error_Handler();
	}

}

/**
  * @brief  Function implementing RX ADC samples captured after fsk transmision.
  * @param  None
  * @retval None
  */
void fillAndInterpolateFskAndRxBuffers(void)
{
	int f1_index = 0;
	int f2_index = 0;
	int last_f1_index = 0;
	int last_f2_index = 0;
	uint32_t step_offset = first_half_data_ready ? 0 : ADC_CONVERTED_DATA_BUFFER_STEP_OFFSET;


	if ( (first_half_data_ready && !first_half_fft_done) || (second_half_data_ready && !second_half_fft_done) ) {
		for (int i = 0; i < ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL; i++) {

			fsk[i] = (float) aADCxConvertedData[ADC_FSK_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
			rx1[2 * i] = (float) aADCxConvertedData[ADC_RX1_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
			rx1[2 * i + 1] = (float) aADCxConvertedData[ADC_RX1_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
			rx2[2 * i] = (float) aADCxConvertedData[ADC_RX2_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
			rx2[2 * i + 1] = (float) aADCxConvertedData[ADC_RX2_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

			if (fsk[i] < FSK_THRESHOLD && f1_index < (ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2)) {
				rx1_f1[2 * i] = (float) aADCxConvertedData[ADC_RX1_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx1_f1[2 * i + 1] = (float) aADCxConvertedData[ADC_RX1_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f1[2 * i] = (float) aADCxConvertedData[ADC_RX2_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f1[2 * i + 1] = (float) aADCxConvertedData[ADC_RX2_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

				//TODO CMSIS DSP interpolation
				if ((i - last_f1_index) > 1) {
					float rx1_f1_i_linear_slope = ((rx1_f1[2 * i] - rx1_f1[2 * last_f1_index])/(i - last_f1_index));
					float rx1_f1_q_linear_slope = ((rx1_f1[2 * i + 1] - rx1_f1[2 * last_f1_index + 1])/(i - last_f1_index));
					float rx2_f1_i_linear_slope = ((rx2_f1[2 * i] - rx2_f1[2 * last_f1_index])/(i - last_f1_index));
					float rx2_f1_q_linear_slope = ((rx2_f1[2 * i + 1] - rx2_f1[2 * last_f1_index + 1])/(i - last_f1_index));

					for (int j = last_f1_index + 1; j < i; j++) {
						rx1_f1[2 * j] = rx1_f1[2 * last_f1_index] + rx1_f1_i_linear_slope * (j - last_f1_index);
						rx1_f1[2 * j + 1] = rx1_f1[2 * last_f1_index + 1] + rx1_f1_q_linear_slope * (j - last_f1_index);
						rx2_f1[2 * j] = rx2_f1[2 * last_f1_index] + rx2_f1_i_linear_slope * (j - last_f1_index);
						rx2_f1[2 * j + 1] = rx2_f1[2 * last_f1_index + 1] + rx2_f1_q_linear_slope * (j - last_f1_index);
					}
				}
				last_f1_index = i;

			} else if (f2_index < (ADC_CONVERTED_DATA_BUFFER_SIZE_PER_CHANNEL*2)){
				rx1_f2[2 * i] = (float) aADCxConvertedData[ADC_RX1_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx1_f2[2 * i + 1] = (float) aADCxConvertedData[ADC_RX1_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f2[2 * i] = (float) aADCxConvertedData[ADC_RX2_I_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];
				rx2_f2[2 * i + 1] = (float) aADCxConvertedData[ADC_RX2_Q_OFFSET + i * ADC_ENABLED_CHANNEL_COUNT + step_offset];

				//TODO CMSIS DSP interpolation
				if ((i - last_f2_index) > 1) {
					float rx1_f2_i_linear_slope = ((rx1_f2[2 * i] - rx1_f2[2 * last_f2_index])/(i - last_f2_index));
					float rx1_f2_q_linear_slope = ((rx1_f2[2 * i + 1] - rx1_f2[2 * last_f2_index + 1])/(i - last_f2_index));
					float rx2_f2_i_linear_slope = ((rx2_f2[2 * i] - rx2_f2[2 * last_f2_index])/(i - last_f2_index));
					float rx2_f2_q_linear_slope = ((rx2_f2[2 * i + 1] - rx2_f2[2 * last_f2_index + 1])/(i - last_f2_index));

					for (int j = last_f2_index + 1; j < i; j++) {
						rx1_f2[2 * j] = rx1_f2[2 * last_f2_index] + rx1_f2_i_linear_slope * (j - last_f2_index);
						rx1_f2[2 * j + 1] = rx1_f2[2 * last_f2_index + 1] + rx1_f2_q_linear_slope * (j - last_f2_index);
						rx2_f2[2 * j] = rx2_f2[2 * last_f2_index] + rx2_f2_i_linear_slope * (j - last_f2_index);
						rx2_f2[2 * j + 1] = rx2_f2[2 * last_f2_index + 1] + rx2_f2_q_linear_slope * (j - last_f2_index);
					}
				}

				last_f2_index = i;

			}
		}
	}
}

