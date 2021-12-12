/*
 * sensorApp.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "gpioApp.hpp"

// Each <ms> sensor task is ran
#define BUTTON_TASK_INTERVAL 10 // [ms]
#define PRESS_TIME 10
#define HOLD_TIME 150
#define MULTIPLE_PRESS_TIMEOUT 100

// Each state is a check of..
enum buttonState {
  INITIAL_PRESS,
  PRESS_TIMER_EXPIRY,
  WAS_IT_PRESS,
  HOLD_TIMER_EXPIRY,
  WAS_IT_HOLD,
  HOLD_ENOUGH
};

extern "C" {
  extern osTimerId_t buttonPressHandle;
  extern osTimerId_t buttonHoldHandle;
  extern osTimerId_t buttonMultiplePressHandle;

  extern osMessageQueueId_t buttonQueueHandle;
} // extern C close

extern "C" void ButtonTask(void *argument) {
  Gpio button = Gpio(BUTTON_TRIGGER_GPIO_Port, BUTTON_TRIGGER_Pin,
                     0, INPUT, PULLUP);

  buttonState state = INITIAL_PRESS;
  uint8_t pressed = 0;
  bool wasPressed = false;
  bool wasHold = false;
  uint8_t dummy = 0;

  for (;;) { // ---------------------------------------
    switch (state) {
    case INITIAL_PRESS:
      if (button.Read()) {
        osTimerStart(buttonPressHandle, PRESS_TIME);
        state = PRESS_TIMER_EXPIRY;
      }
      break;

    case PRESS_TIMER_EXPIRY:
      if (!(osTimerIsRunning(buttonPressHandle))) {
        state = WAS_IT_PRESS;
      } //else stay here
      break;

    case WAS_IT_PRESS:
      if (button.Read()) {
        osTimerStart(buttonHoldHandle, HOLD_TIME);
        state = HOLD_TIMER_EXPIRY;
      }
      else {
        wasPressed = true;
        state = INITIAL_PRESS;
      }
      break;

    //todo check few times if still button is hold
    case HOLD_TIMER_EXPIRY:
      if (!(osTimerIsRunning(buttonPressHandle))) {
        state = WAS_IT_HOLD;
      } //else stay here
      break;

    case WAS_IT_HOLD:
      if (button.Read()) {
        wasHold = true;
        state = HOLD_ENOUGH;
      }
      else {
        wasPressed = true;
        state = INITIAL_PRESS;
      }
      break;

    case HOLD_ENOUGH:
      if (!(button.Read())) {
        state = INITIAL_PRESS;
      } //else stay and wait until released
      break;
    }

    if (wasPressed) {
      pressed++;
      wasPressed = false;
      osTimerStop(buttonMultiplePressHandle);
      osTimerStart(buttonMultiplePressHandle, MULTIPLE_PRESS_TIMEOUT);
    }
    uint8_t buttonRead = (uint8_t)button.Read();
    osMessageQueuePut(buttonQueueHandle, &buttonRead, 0, 200);

    // Put data only when multiple press has timedout
    if (!(osTimerIsRunning)) {
      osMessageQueuePut(buttonQueueHandle, &pressed, 0, 200);
      osMessageQueuePut(buttonQueueHandle, &wasHold, 0, 200);
      pressed = 0;
      wasHold = 0;
    }
    else {
      osMessageQueuePut(buttonQueueHandle, &dummy, 0, 200);
      osMessageQueuePut(buttonQueueHandle, &dummy, 0, 200);
    }
  }
} // ---------------------------------------------------

extern "C" void buttonPressCallback(void *argument) {
  osTimerStop(buttonPressHandle);
}

extern "C" void buttonHoldCallback(void *argument) {
  osTimerStop(buttonHoldHandle);
}

extern "C" void buttonMultiplePressCallback(void *argument) {
  osTimerStop(buttonMultiplePressHandle);
}