/*
 * gpioApp.h
 *
 *  Created on: Aug 11, 2021
 *      Author: macsli
 */

#ifndef INC_GPIOAPP_H_
#define INC_GPIOAPP_H_

#include "stdint.h"
#include "stm32l1xx_hal.h"

enum Mode {
  INPUT,
  OUTPUT,
};

enum Pull {
  NO_PULL,
  PULLUP,
  PULLDOWN,
};

class Gpio {
private:
  uint16_t pin;
  GPIO_TypeDef *port;

public:
  Gpio();

  Gpio(GPIO_TypeDef *_port, uint32_t _pin, bool initState, uint32_t mode,
       uint32_t pull);

  void Set(void);

  void Set(bool value);

  void Reset(void);
};

#endif /* INC_GPIOAPP_H_ */