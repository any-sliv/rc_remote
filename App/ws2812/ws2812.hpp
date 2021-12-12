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

class bufferWrapper {
 private:
  uint16_t buffer1[24];
  uint16_t buffer2[24];

  uint8_t currentBit = 0;

  // false - buffer1; true - buffer2
  bool isActive = false;

 public:
  bufferWrapper() {};

  void append(bool val) {
    //todo too complicated
    //val ? active()[currentBit] = WS_ONE : active()[currentBit] = WS_ZERO;
    currentBit++;
    if (currentBit >= (24 - 1)) currentBit = 0;
  }

  /**
   * Returns address of active buffer
   */
  uint16_t* active(void) { return isActive ? buffer1 : buffer2; }

  void flip(void) { isActive ^= (bool)1; }
};  // -------------------------------------

class Leds {
 private:
  const uint32_t timeout = 2000;
  uint8_t numberOfLeds;

  SPI_HandleTypeDef* ledSpi;

  /**
   * DC/DC supplying leds pin state 0 - down, 1 - up
   */
  Gpio* ledsEnablePin = NULL;

  bufferWrapper buffer;

  struct ws2812_diode_s wsLed[WS2812_LEDS_NUMBER];

  uint8_t currentLed;

 public:
  Leds();
  ~Leds();

  void Powerdown(void);

  /**
   * Loads data for next two diodes, use only in callback
   */
  uint16_t* loadBuffer(void);

  /**
   * Put desired colour, in particular led into diodes structure.
   * Refresh required - do it manually!
   */
  void SetColour(ws2812_diode_s wsStruct, uint8_t ledNumber);

  void Clear(void);

  void Refresh(void);

  uint8_t GetCurrentLed(void);
};  // -------------------------------------