/*
 * ws2812.hpp
 *
 *  Created on: Aug 11, 2021
 *      Author: macsli
 */

#include "gpioApp.hpp"
#include "stdint.h"
#include "stm32l1xx_hal.h"
// gpio.h used only for pins naming - sourced from CubeMX
#include "gpio.h"

#define WS2812_LEDS_NUMBER 5

#define WS_ZERO (const uint16_t)0b1111100000000000
#define WS_ONE (const uint16_t)0b1111111111000000

struct ws2812_diode_s {
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
};  // -------------------------------------

class Buffer {
 public:
  Buffer() {};
  uint16_t tx[24 * WS2812_LEDS_NUMBER];
  size_t len;

  /**
   * Appends a @param bit to buffer
   */
  void append(bool bit) {
    bit ? tx[len] = WS_ONE : tx[len] = WS_ZERO;
    len++;
  }
};

class Leds {
 private:
  const uint32_t timeout = 2000;
  uint8_t numberOfLeds;

  SPI_HandleTypeDef* ledSpi;

  Buffer txBuffer;

  /**
   * DC/DC supplying leds pin state 0 - down, 1 - up
   */
  Gpio ledsEnablePin;

  struct ws2812_diode_s wsLed[WS2812_LEDS_NUMBER];

  uint8_t currentLed;

  void loadBuffer(void);

  void Refresh(void);
  
 public:
  Leds();
  ~Leds();

  void Powerdown(void);

  /**
   * Put desired colour, in particular led into diodes structure.
   * Refresh required - do it manually!
   */
  void SetColour(ws2812_diode_s wsStruct, uint8_t ledNumber);

  void Clear(void);


  uint8_t GetCurrentLed(void);
};  // -------------------------------------