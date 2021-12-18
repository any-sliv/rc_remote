/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for radio */
osThreadId_t radioHandle;
const osThreadAttr_t radio_attributes = {
  .name = "radio",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for sensor */
osThreadId_t sensorHandle;
const osThreadAttr_t sensor_attributes = {
  .name = "sensor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for button */
osThreadId_t buttonHandle;
const osThreadAttr_t button_attributes = {
  .name = "button",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for qButtonPresses */
osMessageQueueId_t qButtonPressesHandle;
const osMessageQueueAttr_t qButtonPresses_attributes = {
  .name = "qButtonPresses"
};
/* Definitions for qButtonHold */
osMessageQueueId_t qButtonHoldHandle;
const osMessageQueueAttr_t qButtonHold_attributes = {
  .name = "qButtonHold"
};
/* Definitions for qButtonState */
osMessageQueueId_t qButtonStateHandle;
const osMessageQueueAttr_t qButtonState_attributes = {
  .name = "qButtonState"
};
/* Definitions for qRadioTxValue */
osMessageQueueId_t qRadioTxValueHandle;
const osMessageQueueAttr_t qRadioTxValue_attributes = {
  .name = "qRadioTxValue"
};
/* Definitions for radioHeartbeat */
osTimerId_t radioHeartbeatHandle;
const osTimerAttr_t radioHeartbeat_attributes = {
  .name = "radioHeartbeat"
};
/* Definitions for ledTimeout */
osTimerId_t ledTimeoutHandle;
const osTimerAttr_t ledTimeout_attributes = {
  .name = "ledTimeout"
};
/* Definitions for buttonPress */
osTimerId_t buttonPressHandle;
const osTimerAttr_t buttonPress_attributes = {
  .name = "buttonPress"
};
/* Definitions for buttonHold */
osTimerId_t buttonHoldHandle;
const osTimerAttr_t buttonHold_attributes = {
  .name = "buttonHold"
};
/* Definitions for buttonMultiplePress */
osTimerId_t buttonMultiplePressHandle;
const osTimerAttr_t buttonMultiplePress_attributes = {
  .name = "buttonMultiplePress"
};
/* Definitions for radioTxDoneSemaphore */
osSemaphoreId_t radioTxDoneSemaphoreHandle;
const osSemaphoreAttr_t radioTxDoneSemaphore_attributes = {
  .name = "radioTxDoneSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void RadioTask(void *argument);
extern void SensorTask(void *argument);
extern void ButtonTask(void *argument);
extern void radioHeartbeatCallback(void *argument);
extern void ledTimeoutCallback(void *argument);
extern void buttonPressCallback(void *argument);
extern void buttonHoldCallback(void *argument);
extern void buttonMultiplePressCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of radioTxDoneSemaphore */
  radioTxDoneSemaphoreHandle = osSemaphoreNew(1, 1, &radioTxDoneSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of radioHeartbeat */
  radioHeartbeatHandle = osTimerNew(radioHeartbeatCallback, osTimerPeriodic, NULL, &radioHeartbeat_attributes);

  /* creation of ledTimeout */
  ledTimeoutHandle = osTimerNew(ledTimeoutCallback, osTimerOnce, NULL, &ledTimeout_attributes);

  /* creation of buttonPress */
  buttonPressHandle = osTimerNew(buttonPressCallback, osTimerOnce, NULL, &buttonPress_attributes);

  /* creation of buttonHold */
  buttonHoldHandle = osTimerNew(buttonHoldCallback, osTimerOnce, NULL, &buttonHold_attributes);

  /* creation of buttonMultiplePress */
  buttonMultiplePressHandle = osTimerNew(buttonMultiplePressCallback, osTimerOnce, NULL, &buttonMultiplePress_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of qButtonPresses */
  qButtonPressesHandle = osMessageQueueNew (1, sizeof(uint8_t), &qButtonPresses_attributes);

  /* creation of qButtonHold */
  qButtonHoldHandle = osMessageQueueNew (1, sizeof(uint8_t), &qButtonHold_attributes);

  /* creation of qButtonState */
  qButtonStateHandle = osMessageQueueNew (1, sizeof(uint8_t), &qButtonState_attributes);

  /* creation of qRadioTxValue */
  qRadioTxValueHandle = osMessageQueueNew (1, sizeof(int), &qRadioTxValue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of radio */
  radioHandle = osThreadNew(RadioTask, NULL, &radio_attributes);

  /* creation of sensor */
  sensorHandle = osThreadNew(SensorTask, NULL, &sensor_attributes);

  /* creation of button */
  buttonHandle = osThreadNew(ButtonTask, NULL, &button_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
