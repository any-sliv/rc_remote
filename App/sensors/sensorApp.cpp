/*
 * sensorApp.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "task.h"
#include "buttonApp.hpp"
#include "battery.hpp"
#include "ss49.hpp"
#include "logger.hpp"

/*  Each <ms> task is ran. This must be lower than 
    #def <buttonApp.cpp>/BUTTON_TASK_INTERVAL */
#define SENSOR_TASK_INTERVAL 5 // [ms]

extern "C" void SensorTask(void * argument) {
    // Data from hallSensor sent to radio
  SS49 hallSensor;
    // Data from battery sent to LEDs
  Battery battery;
    // State of trigger button
  uint8_t triggerButtonState = 0;
  uint8_t timesPressed = 0;
  uint8_t triggerButtonHold = 0;

  for(;;) {
    xQueueReceive(qButtonStateHandle, &triggerButtonState, 0);
    xQueueReceive(qButtonPressesHandle, &timesPressed, 0);
    xQueueReceive(qButtonHoldHandle, &triggerButtonHold, 0);

    //if(timesPressed > 1) Logger::Log("Pressed x times :D", 32);
    vTaskDelay(SENSOR_TASK_INTERVAL);
  }
}