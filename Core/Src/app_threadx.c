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
#include "fsk_generator.h"
#include "fsk_processing.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
  TX_THREAD ThreadSamplingCapture;
  TX_THREAD ThreadSignalProcessing;
  TX_EVENT_FLAGS_GROUP EventFlag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
  void ThreadSamplingCapture_Entry(ULONG thread_input);
  void ThreadSignalProcessing_Entry(ULONG thread_input);
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

  /* USER CODE END App_ThreadX_MEM_POOL */
CHAR *pointer;

  /* Allocate the stack for Main thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
   /* Create Main thread.  */
  if (tx_thread_create(&tx_app_thread, "Main thread", MainThread_thread_entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  /* Allocate the stack for ThreadFsk.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create ThreadOne.  */
 if (tx_thread_create(&ThreadSamplingCapture, "Thread Sampling Capture", ThreadSamplingCapture_Entry, 0,
                      pointer,  TX_APP_STACK_SIZE,
					THREAD_SAMPLING_CAPTURE_PRIO, THREAD_SAMPLING_CAPTURE_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
 }

 /* Allocate the stack for ThreadSignalProcessing.  */
 if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
 {
   ret = TX_POOL_ERROR;
 }

 /* Create ThreadTwo.  */
 if (tx_thread_create(&ThreadSignalProcessing, "Thread SignalProcessing", ThreadSignalProcessing_Entry, 0,
                      pointer,  TX_APP_STACK_SIZE,
					THREAD_SIGNAL_PROCESSING_PRIO, THREAD_SIGNAL_PROCESSING_PREEMPTION_THRESHOLD,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
 {
   ret = TX_THREAD_ERROR;
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
  * @brief  Function implementing the MainThread_thread_entry thread.
  * @param  thread_input: Not used.
  * @retval None
  */
void MainThread_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_thread_entry */
	(void) thread_input;


	startFskTransmission();

  /* Infinite loop */
  while(1)
  {
		//printf("Main Thread execution!\r\n");
		tx_thread_sleep(100);
  }

  tx_thread_terminate(&ThreadSamplingCapture);
  tx_thread_terminate(&ThreadSignalProcessing);

  /* USER CODE END MainThread_thread_entry */
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
  * @brief  Function implementing the ThreadSamplingCapture thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadSamplingCapture_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
  (void) thread_input;

  /* Infinite loop */
  while(1)
  {

	  //tx_thread_sleep(10);

	  if (tx_event_flags_get(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR_CLEAR,
	  		                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS) {
	  	Error_Handler();
	  } else {
	  	printf("Sampling thread execution!\r\n");
	  }


/*	  if (tx_event_flags_set(&EventFlag, THREAD_SAMPLING_CAPTURE_EVT, TX_OR) != TX_SUCCESS)
	  {
			Error_Handler();
	  }
*/
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
  (void) thread_input;


  /* Infinite loop */
  while(1)
  {

	  //tx_thread_sleep(50);

	  if (tx_event_flags_get(&EventFlag, THREAD_SIGNAL_PROCESSING_EVT, TX_OR_CLEAR,
	  		                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS) {
	  	Error_Handler();
	  } else {
	  	printf("Processing thread execution!\r\n");
	  }

/*	  if (tx_event_flags_set(&EventFlag, THREAD_SIGNAL_PROCESSING_EVT, TX_OR) != TX_SUCCESS)
	  {
			Error_Handler();
	  }
*/
  }
}
/* USER CODE END 1 */
