/*
 * buttonApp.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "gpioApp.hpp"
#include "logger.hpp"

// Each <ms> sensor task is ran
#define BUTTON_TASK_INTERVAL 5 // [ms]
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
  HOLD_ENOUGH,
  END
};

enum buttonStatus {
  RELEASED,
  PRESSED,
  HOLD
};

extern "C" {
  extern osTimerId_t buttonPressHandle;
  extern osTimerId_t buttonHoldHandle;
  extern osTimerId_t buttonMultiplePressHandle;

  extern osMessageQueueId_t qButtonPressesHandle;
  extern osMessageQueueId_t qButtonHoldHandle;
  extern osMessageQueueId_t qButtonStateHandle;
} // extern C close

extern "C" void ButtonTask(void * argument) {
  Gpio button = Gpio(BUTTON_TRIGGER_GPIO_Port, BUTTON_TRIGGER_Pin,
                     0, INPUT, PULLUP);

  // State machine state
  buttonState state = INITIAL_PRESS;
  // Button current status
  buttonStatus status = RELEASED;

  uint8_t pressesNumber = 0;
  uint8_t dummy = 0;

  for (;;) { // ---------------------------------------
    switch (state) {
      status = RELEASED;

    case INITIAL_PRESS:
      if (button.Read()) {
      //if(!(HAL_GPIO_ReadPin(BUTTON_TRIGGER_GPIO_Port, BUTTON_TRIGGER_Pin))) {
        osTimerStart(buttonPressHandle, PRESS_TIME);
        state = PRESS_TIMER_EXPIRY;
      }
      break;

    case PRESS_TIMER_EXPIRY:
      // Press timer expired?
      if (!(osTimerIsRunning(buttonPressHandle))) {
        state = WAS_IT_PRESS;
      }
      break;

    case WAS_IT_PRESS:
      if (button.Read()) {
        osTimerStart(buttonHoldHandle, HOLD_TIME);
        state = HOLD_TIMER_EXPIRY;
      }
      else {
        state = END;
        status = PRESSED;
      }
      break;

    case HOLD_TIMER_EXPIRY:
      // Hold timer expired?
      if (!(osTimerIsRunning(buttonPressHandle))) {
        state = WAS_IT_HOLD;
        status = HOLD;
      }
      else {
        // If released
        if (! (button.Read())) {
          state = END;
          status = PRESSED;
        }
      }
      break;

    case HOLD_ENOUGH:
      // If released
      if (! (button.Read())) {
        state = END;
      } //else stay and wait until released
      break;

    case END:
      state = INITIAL_PRESS;
      break;
    }

    // Each press increase number of presses and run timer
    // Pressed is cleared when timer expires.
    // So as long as its running you can add presses.
    if (status == PRESSED) {
      pressesNumber++;
      osTimerStart(buttonMultiplePressHandle, MULTIPLE_PRESS_TIMEOUT);
      // Delay due to OS must tick to detect if timer runs.
      osDelay(1);
    }

    // Warning: if queue if full no messages are put
    uint8_t buttonRead = (uint8_t)button.Read();
    // Queue send! Current button state.
    xQueueSend(qButtonStateHandle, &buttonRead, 0);

    //Put data only when multiple press has timedout
    if (!(osTimerIsRunning(buttonMultiplePressHandle))) {
      // Queue send! Number of presses.
      xQueueSend(qButtonPressesHandle, &pressesNumber, 0);
      pressesNumber = 0;
    }

    if(status == HOLD) {
      // Queue send! If button is hold.
      xQueueSend(qButtonHoldHandle, &status, 0);
    }

    osDelay(BUTTON_TASK_INTERVAL);
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