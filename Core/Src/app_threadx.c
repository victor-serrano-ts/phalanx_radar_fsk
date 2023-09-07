/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "sampling_capture.h"
#include "fsk_generator.h"
#include "fsk_processing.h"
#include "events.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/** @brief RADAR control state machine states. */
typedef enum
{
//    FSK_RADAR_ST_TURN_ON = 0,
//    FSK_RADAR_ST_IDLE,
    FSK_RADAR_ST_DETECTING,
    FSK_RADAR_ST_DETECTED,
//    FSK_RADAR_ST_SLEEP,
} fsk_radar_st_t;

/** Radar control state machine current state. */
static fsk_radar_st_t fsk_radar_st;

/** Radar event detected. */
static event_t fsk_event_result;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
  TX_THREAD tx_app_thread;
/* USER CODE BEGIN PV */
  TX_THREAD ThreadSignalGenerator;
  TX_THREAD ThreadSamplingCapture;
  TX_THREAD ThreadSignalProcessing;
  TX_THREAD ThreadEventDetector;
  TX_EVENT_FLAGS_GROUP EventFlag;

  TX_QUEUE tx_app_msg_queue;
  //TX_QUEUE MsgQueueTwo;

  static uint32_t adc_total_time_seconds = 0;
  static uint32_t adc_total_rate_ksps = 0;
  static uint32_t adc_channel_rate_ksps = 0;
  static uint32_t ffts_per_second = 0;

  static uint32_t counter_signal = 0;
  static uint32_t counter_sampling = 0;
  static uint32_t counter_processing = 0;
  static uint32_t counter_detector = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
  void ThreadSignalGenerator_Entry(ULONG thread_input);
  void ThreadSamplingCapture_Entry(ULONG thread_input);
  void ThreadSignalProcessing_Entry(ULONG thread_input);
  void ThreadEventDetector_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  //TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  //CHAR *pointer;
  /* USER CODE END App_ThreadX_MEM_POOL */
CHAR *pointer;

  /* Allocate the stack for Main Thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
   /* Create Main Thread.  */
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */

  /* Allocate the stack for ThreadFsk.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
		 TX_SIGNAL_GENERATOR_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create SignalGenerator.  */
 if (tx_thread_create(&ThreadSignalGenerator, "Thread Signal Generator", ThreadSignalGenerator_Entry, 0,
                      pointer,  TX_SIGNAL_GENERATOR_STACK_SIZE,
											THREAD_SIGNAL_GENERATOR_PRIO, THREAD_SIGNAL_GENERATOR_PRIO_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
 }

 /* Allocate the stack for ThreadSamplingCapture.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
		 TX_SAMPLING_CAPTURE_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create SamplingCapture.  */
 if (tx_thread_create(&ThreadSamplingCapture, "Thread Sampling Capture", ThreadSamplingCapture_Entry, 0,
                      pointer,  TX_SAMPLING_CAPTURE_STACK_SIZE,
											THREAD_SAMPLING_CAPTURE_PRIO, THREAD_SAMPLING_CAPTURE_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
 }

 /* Allocate the stack for ThreadSignalProcessing.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
		 TX_SIGNAL_PROCESSING_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create SignalProcessing.  */
 if (tx_thread_create(&ThreadSignalProcessing, "Thread SignalProcessing", ThreadSignalProcessing_Entry, 0,
                      pointer,  TX_SIGNAL_PROCESSING_STACK_SIZE,
											THREAD_SIGNAL_PROCESSING_PRIO, THREAD_SIGNAL_PROCESSING_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
 }

 /* Allocate the stack for ThreadEventDetector.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
		 TX_EVENT_DETECTOR_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create EventDetector.  */
 if (tx_thread_create(&ThreadEventDetector, "Thread EventDetector", ThreadEventDetector_Entry, 0,
                      pointer,  TX_EVENT_DETECTOR_STACK_SIZE,
											THREAD_EVENT_DETECTOR_PRIO, THREAD_EVENT_DETECTOR_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
 }

 /* Create the tx_app_msg_queue shared by MsgSenderThreadTwo and MsgReceiverThread.  */
 if (tx_queue_create(&tx_app_msg_queue, "App Mssg Queue", TX_APP_SINGLE_MSG_SIZE,
                     pointer, TX_APP_MSG_QUEUE_FULL_SIZE *sizeof(ULONG)) != TX_SUCCESS)
 {
   ret = TX_QUEUE_ERROR;
 }

 /* Create the event flags group.  */
 if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
 {
   ret = TX_GROUP_ERROR;
 }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the MainThread_entry thread.
  * @param  thread_input: Not used.
  * @retval None
  */
void MainThread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_entry */
/*	startFskTransmission();
	startFskSamplingCapture();
	initFftModule();
*/

	/* Infinite loop */
	while(1)
	{
		tx_thread_sleep(1000);
		adc_total_time_seconds++;
		adc_total_rate_ksps = adc_full_count * ADC_CONVERTED_DATA_BUFFER_SIZE / (adc_total_time_seconds);
		adc_channel_rate_ksps = adc_total_rate_ksps / 5;
		ffts_per_second = total_ffts / adc_total_time_seconds;
		printf("ADC channel rate: %lu, FFTs rate: %lu\r\n", adc_channel_rate_ksps, ffts_per_second);
		printf("Signal: %lu, Sampling: %lu, Processsing: %lu, Detector: %lu\r\n", counter_signal, counter_sampling, counter_processing, counter_detector);
		//printf("adc_half_count: %lu, adc_full_count: %lu\r\n",adc_half_count, adc_full_count);
		counter_signal = 0;
		counter_sampling = 0;
		counter_processing = 0;
		counter_detector = 0;

		//adc_half_count = 0;
		//adc_full_count = 0;
	}
  /* USER CODE END MainThread_entry */
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the ThreadSignalGenerator thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadSignalGenerator_Entry(ULONG thread_input)
{
	//ULONG   actual_flags = 0;

  start_fsk_transmission();
  counter_signal++;

  /* Infinite loop */
/*  while(1)
  {
  	if (tx_event_flags_get(&EventFlag, THREAD_SIGNAL_GENERATOR_EVT, TX_OR_CLEAR,
  		  		                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
  	{
  		  	Error_Handler();
  	}
  	else
  	{
  		//printf("Signal generator thread execution!\r\n");

  		counter_signal++;

  		//TODO - Generation actions
  	}
  }
*/
}

/**
  * @brief  Function implementing the ThreadSamplingCapture thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadSamplingCapture_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;

  start_fsk_sampling_capture();

  /* Infinite loop */
  while(1)
  {
	  if (tx_event_flags_get(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR_CLEAR,
	  		                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
	  {
	  	Error_Handler();
	  }
	  else
	  {
	  	//TODO - Sampling Capture actions
	  	//printf("Sampling capture thread execution!\r\n");
	  	counter_sampling++;

	  	fill_and_interpolate_fsk_rx_buffers();
	    if (tx_event_flags_set(&EventFlag, THREAD_SIGNAL_PROCESSING_EVT, TX_OR) != TX_SUCCESS)
	  	{
	  		Error_Handler();
	  	}
	  }
  }
}

/**
  * @brief  Function implementing the ThreadSignalProcessing thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadSignalProcessing_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
	fsk_result_t detection_result_snd;


  init_fft_module();

  /* Infinite loop */
  while(1)
  {
	  if (tx_event_flags_get(&EventFlag, THREAD_SIGNAL_PROCESSING_EVT, TX_OR_CLEAR,
	  		                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
	  {
	  	Error_Handler();
	  }
	  else
	  {
	  	// TODO - Processing signal actions
	  	//printf("Processing signal thread execution!\r\n");
	  	counter_processing++;
	  	detection_result_snd = get_detection_parameters();

	  	if (first_half_data_ready) {
				first_half_fft_done = true;
			} else {
				second_half_fft_done = true;
			}
	  	/* Send message to ThreadEventDetector.  */
			if (tx_queue_send(&tx_app_msg_queue, &detection_result_snd, TX_NO_WAIT/*TX_WAIT_FOREVER*/) != TX_SUCCESS)
			{
				Error_Handler();
			}
	  }
  }
}

/**
  * @brief  Function implementing the ThreadEventDetector thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadEventDetector_Entry(ULONG thread_input)
{
	fsk_result_t detection_result_rcv;
	UINT status = 0;

	set_default_params();

	fsk_radar_st = FSK_RADAR_ST_DETECTING;

  /* Infinite loop */
  while(1)
  {
  	counter_detector++;
    status = tx_queue_receive(&tx_app_msg_queue, &detection_result_rcv, TX_WAIT_FOREVER/*TX_NO_WAIT*/);
    if ( status == TX_SUCCESS)
    {
//    	printf("bin: %f, freq: %f, angel: %f, speed: %f, distance: %f, motion: %lu, noise: %f\r\n",
//    			detection_result_rcv.bin_level,
//					detection_result_rcv.frequency_hz,
//					detection_result_rcv.angle,
//					detection_result_rcv.speed_kmh,
//					detection_result_rcv.distance,
//					detection_result_rcv.motion,
//					detection_result_rcv.noise_level);
    	//printf("Mssg_Rcv!!\r\n");

    	switch(fsk_radar_st)
    	{
				case FSK_RADAR_ST_DETECTING:

					// Detect possible events
					fsk_event_result = event_detect(&detection_result_rcv, 100);
          // If an event is detected, save more fsk samples before saving
          // the event
          if (fsk_event_result != EVENT_NONE)
          {
						if (fsk_event_result== EVENT_FAST) {
							printf("fast_event\r\n");
						} else {
							fsk_radar_st = FSK_RADAR_ST_DETECTED;
						}
          }
					break;

				case FSK_RADAR_ST_DETECTED:

					fsk_event_result = wait_for_more_events(&detection_result_rcv, 100);

					if((fsk_event_result != EVENT_NONE) && (fsk_event_result != EVENT_FAST)) {
						printf("event %d detected!\r\n", fsk_event_result);
					}
					else
					{
						fsk_radar_st = FSK_RADAR_ST_DETECTED;
					}
					break;
    	}
    }
  }
}
/* USER CODE END 1 */
