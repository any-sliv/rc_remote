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

// Each <ms> task is ran
#define SENSOR_TASK_INTERVAL 100 // [ms]

extern "C" void SensorTask(void * argument) {
    // Data from hallSensor sent to radio
  SS49 hallSensor;
    // Data from battery sent to LEDs
  Battery battery;

  for(;;) {

    vTaskDelay(SENSOR_TASK_INTERVAL);
  }
}