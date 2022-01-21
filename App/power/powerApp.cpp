/*
 * radioApp.hpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "gpioApp.hpp"

extern "C" {
  extern osTimerId_t radioHeartbeatHandle;
  extern osTimerId_t ledTimeoutHandle;
  extern IWDG_HandleTypeDef hiwdg;
}

extern "C" void PowerManagementTask(void * argument) {

  for(;;) {
    HAL_IWDG_Refresh(&hiwdg);

    if(!osTimerIsRunning(radioHeartbeatHandle) && 
        !osTimerIsRunning(ledTimeoutHandle)) {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
    }
  }
  vTaskDelay(500);
}