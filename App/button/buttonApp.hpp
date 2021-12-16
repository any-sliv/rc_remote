/*
 * buttonApp.hpp
 *
 *  Created on: Dec 12, 2021
 *      Author: macsli
 */

#ifndef __BUTTON_APP_HPP
#define __BUTTON_APP_HPP

#include "cmsis_os.h"
#include "queue.h"

extern "C" {
  extern osMessageQueueId_t qButtonPressesHandle;
  extern osMessageQueueId_t qButtonHoldHandle;
  extern osMessageQueueId_t qButtonStateHandle;
} // extern C close

#endif