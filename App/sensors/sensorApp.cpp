/*
 * sensorApp.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "buttonApp.hpp"
#include "battery.hpp"
#include "ss49.hpp"
#include "logger.hpp"

/*  Each <ms> task is ran. */
#define SENSOR_TASK_INTERVAL 5 // [ms]
// Amount of presses to trigger action
#define INTERNAL_BATTERY_TRIGGER 3
#define EXTERNAL_BATTERY_TRIGGER 5

extern "C" void SensorTask(void * argument) {
    // Data from hallSensor sent to radio
  SS49 hallSensor;
    // Data from battery sent to LEDs
  Battery battery;
    // State of trigger button
  uint8_t triggerButtonState = 0;
  uint8_t timesPressed = 0;
  uint8_t triggerButtonHold = 0;

    // Used to calculate average from hallSensor
  int sum = 0, counter = 0, avg = 0;

  for(;;) {
    xQueueReceive(qButtonStateHandle, &triggerButtonState, 0);
    xQueueReceive(qButtonPressesHandle, &timesPressed, 0);
    xQueueReceive(qButtonHoldHandle, &triggerButtonHold, 0);

    // Anti overflow
    if(sum >= 0xFFFF0000) sum = 0;
    if(sum >= 0xFFFF0000) counter = 0;
    //sum += hallSensor.GetPosition();
    counter++;

    if(xSemaphoreTake(radioTxDoneSemaphoreHandle, 1) == pdTRUE) {
      avg = sum/counter;
      // Send calculated average
      xQueueSend(qRadioTxValueHandle, &avg, 0);
      sum = 0;
    }

    if(timesPressed == INTERNAL_BATTERY_TRIGGER) {
    //todo send it to ws2812 led app
      uint8_t pt = battery.GetPercent();
      int a = 0;
    }

    //todo TEST ME
    bool chrg = battery.IsCharging();

    //if(timesPressed > 1) Logger::Log("Pressed x times :D", 32);
    vTaskDelay(SENSOR_TASK_INTERVAL);
  }
}