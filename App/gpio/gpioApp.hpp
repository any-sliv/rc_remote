/*
 * gpioApp.h
 *
 *  Created on: Aug 11, 2021
 *      Author: macsli
 */

#ifndef INC_GPIOAPP_H_
#define INC_GPIOAPP_H_

#include "stm32l1xx_hal_gpio.h"
#include "stdint.h"

#define INIT_LOW  0
#define INIT_HIGH 1


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
  /**
   * Gpio class constructor. Instantize if want to
   * initialize GPIO pin.
   *
   * @param _port Port of GPIO pin.
   * @param _pin cmon thats self explanatorya
   * @param initState State which pin is given at init
   * @param mode Input/output
   * @param pull pull type
   */
  Gpio(GPIO_TypeDef *_port, uint32_t _pin, bool initState = 0,
           uint32_t mode = OUTPUT, uint32_t pull = NO_PULL);

  // Sets pin value to '1'
  void Set(void);

  /**
   * Sets pin value to @param value
   */
  void Set(bool value);

  // Sets pin value to '0'
  void Reset(void);
};

#endif /* INC_GPIOAPP_H_ */