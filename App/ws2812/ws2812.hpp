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
#include "string.h"

#define WS2812_LEDS_NUMBER 5

struct ws2812_diode_s {
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
};  // -------------------------------------

class Buffer {
 public:
  Buffer() {
    memset(&tx, 0, sizeof(tx)/sizeof(tx[0]));
  };
  uint16_t tx[24 * WS2812_LEDS_NUMBER];
  size_t len = 0;

  /**
   * Appends a @param bit to buffer
   */
  void append(bool bit) {
    if(bit) tx[len] = ws_one;
    else tx[len] = ws_zero;

    len++;
    if(len >= 120) len = 0; 
  }

 private:
  const uint16_t ws_zero = 0b1111000000000000;
  const uint16_t ws_one = 0b1111111111100000;
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

    /**
   * Put desired colour, in particular led into diodes structure.
   * Refresh required - do it manually!
   */
  void SetColour(ws2812_diode_s wsStruct, uint8_t ledNumber);

  /**
   * @param percent percent of battery
   * @return Number of leds to light up
   */
  uint8_t convertPercentToLeds(uint8_t percent);
  
 public:
  Leds();
  ~Leds();

  void Refresh(void);
  
  void Powerdown(void);

  /**
   * If battery has low voltage one diode will blink periodically
   */
  void IndicateLowBattery(void);

  /**
   * Diodes indicate (animation) current battery charge. 
   * Keep calling this function until retruns true. 
   * Best animation if called each 4 ms 
   * @param batteryPercent percent of charge
   * @return Flag if animation has finished 
   */
  bool ShowBatteryState(uint8_t internalPercent, uint8_t externalPercent);


  void Clear(void);

 private:
    const uint8_t _gammaTable[256] = {
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,
     2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,
     3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,
     5,   5,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   8,   8,   8,   8,
     9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  13,
    13,  13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,
    18,  18,  19,  19,  19,  20,  20,  21,  21,  21,  22,  22,  23,  23,  23,  24,
    24,  25,  25,  25,  26,  26,  27,  27,  28,  28,  28,  29,  29,  30,  30,  31,
    31,  32,  32,  33,  33,  34,  34,  35,  35,  36,  36,  37,  37,  38,  38,  39,
    39,  40,  41,  41,  42,  42,  43,  43,  44,  44,  45,  46,  46,  47,  47,  48,
    49,  49,  50,  50,  51,  52,  52,  53,  54,  54,  55,  56,  56,  57,  58,  58,
    59,  60,  60,  61,  62,  62,  63,  64,  64,  65,  66,  67,  67,  68,  69,  70,
    70,  71,  72,  73,  73,  74,  75,  76,  76,  77,  78,  79,  79,  80,  81,  82,
    83,  84,  84,  85,  86,  87,  88,  89,  89,  90,  91,  92,  93,  94,  95,  95,
    96,  97,  98,  99, 100, 101, 102, 103, 103, 104, 105, 106, 107, 108, 109, 110,
  };

  uint8_t gamma8(uint8_t x);
};  // -------------------------------------

