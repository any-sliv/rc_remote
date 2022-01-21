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
#include "ws2812.hpp"
#include "flash.hpp"

/*  Each <ms> task is ran. */
#define SENSOR_TASK_INTERVAL 5 // [ms]

// Amount of presses to trigger action
#define INDICATE_BATTERY_TRIGGER 3
#define INDICATE_RIDE_MODE 5

//todo remove -no-rrti and exceptions, try if it runs

extern "C" void SensorTask(void * argument) {
    // WS2812 leds entity
  Leds leds;
    // Data from hallSensor sent to radio
  SS49 hallSensor;
  
  { // Dummy scope
    uint32_t tmpMode = 0;
    Flash_Read_Data(0x0801F000, &tmpMode, 1);
    hallSensor.ChangeRideMode(tmpMode);
  }
    // Data from battery sent to LEDs
  Battery battery;
    // State of trigger button
  uint8_t triggerButtonState = 0;
  uint8_t timesPressed = 0;
  uint8_t triggerButtonHold = 0;
    //Used to turn on battery state indication
  bool indicateBattery = false;
    //Used to turn on ride mode change indication
  bool indicateRideMode = false;

    // Used to calculate average from hallSensor
  int sum = 0, counter = 0, avg = 0;

  for(;;) {
    xQueueReceive(qButtonStateHandle, &triggerButtonState, 0);
    xQueueReceive(qButtonPressesHandle, &timesPressed, 0);
    xQueueReceive(qButtonHoldHandle, &triggerButtonHold, 0);
    
    sum += hallSensor.GetPosition();
    counter++;

    // Radio sets semaphore after tx transmission.
    if(xSemaphoreTake(radioTxDoneSemaphoreHandle, 1) == pdTRUE) {
      // Send averaged throttle value if trigger is pressed. 
      // Otherwise send idle position -> 0
      triggerButtonState ? avg = 0 : avg = sum/counter;

      xQueueSend(qRadioTxValueHandle, &avg, 0);
      sum = 0, counter = 0;
    }
    
    //Battery::ChargeState batteryState = battery.GetChargeState();
    //todo if charging send to led app
    uint8_t batteryPercent = battery.GetPercent();
    SS49::rideMode mode = hallSensor.ChangeRideMode();

    if(batteryPercent < 15) leds.IndicateLowBattery();
    if(indicateBattery) {
      if(leds.ShowBatteryState(batteryPercent, 50)) indicateBattery = false;
    }
    if(indicateRideMode) {
      if(leds.IndicateRideModeChange(mode)) indicateRideMode = false;
    }

    switch(timesPressed) {
      case INDICATE_BATTERY_TRIGGER:
        // Latches flag until animation returns true and zeroes flag
        indicateBattery = true;
        break;
      case INDICATE_RIDE_MODE:
        indicateRideMode = true;
        uint32_t saveMode = (uint32_t) mode;
        Flash_Write_Data(0x0801F000, &saveMode, 1);
        break;
    }

    if(batteryPercent <= 10) {
      //todo light up leds periodically to indicate low battery
    }
    

    // Anti overflow
    if(sum >= (int)0xFFFF0000) sum = 0;
    if(counter >= (int)0xFFFF0000) counter = 0;

    vTaskDelay(SENSOR_TASK_INTERVAL);
  }
}