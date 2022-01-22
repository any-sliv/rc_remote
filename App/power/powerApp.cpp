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
  //extern IWDG_HandleTypeDef hiwdg;
}

static void enableClockAfterSleep(void);

extern "C" void PowerManagementTask(void * argument) {
  osTimerStart(radioHeartbeatHandle, 5000);
  // Make OS tick to start timer.
  osDelay(50);

  for(;;) {
    //HAL_IWDG_Refresh(&hiwdg);

    if(!osTimerIsRunning(radioHeartbeatHandle) && 
        !osTimerIsRunning(ledTimeoutHandle)) {
      
      //HAL_SuspendTick();
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
      // zzz...

      //wakeup
      enableClockAfterSleep();
      HAL_ResumeTick();
      osDelay(1);
      //HAL_ADC_Stop(&hadc1);
    }
    
    vTaskDelay(500);
  }
}

static void enableClockAfterSleep(void) {
  SystemClock_Config();
}
