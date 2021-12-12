/*
 * sensorApp.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "battery.hpp"
#include "ss49.hpp"
#include "logger.hpp"

/*  Each <ms> task is ran. This must be lower than 
    #def <buttonApp.cpp>/BUTTON_TASK_INTERVAL */
#define SENSOR_TASK_INTERVAL 5 // [ms]

extern "C" {
  extern osMessageQueueId_t buttonQueueHandle;
} // extern C close

extern "C" void SensorTask(void * argument) {
    // Data from hallSensor sent to radio
  SS49 hallSensor;
    // Data from battery sent to LEDs
  Battery battery;
    // State of trigger button
  uint8_t triggerButtonState = 0;

  char * str;
  std::string msg;
  for(;;) {
    osMessageQueueGet(buttonQueueHandle, &triggerButtonState, 0, 0);
    itoa((int)triggerButtonState, str, 10);
    vTaskDelay(SENSOR_TASK_INTERVAL);
  }
}