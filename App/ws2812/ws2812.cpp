/*
 * ws2812App.cpp
 *
 *  Created on: Aug 11, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "logger.hpp"
#include "spi.h"
#include "ws2812.hpp"

extern "C" {
  // Global variables only for callback functions
  extern osTimerId_t ledTimeoutHandle;
  extern SPI_HandleTypeDef hspi2;
}

static Leds * ledsPointer;

//todo take care, data is inverted at v level converter

Leds::Leds() {
  //Logger::Log("WS2812 leds Constructor. \r\n");

  ledsEnablePin = Gpio(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin);
  MX_SPI2_Init();
  ledSpi = &hspi2;
  // Clear tx buffer
  Clear();

  osTimerStart(ledTimeoutHandle, 1000);
  ledsPointer = this;
}

Leds::~Leds() { HAL_SPI_MspDeInit(ledSpi); }

void Leds::SetColour(ws2812_diode_s wsStruct, uint8_t ledNumber) {
  if (ledNumber > WS2812_LEDS_NUMBER) return;
  wsLed[ledNumber] = wsStruct;
}

void Leds::loadBuffer(void) {
  uint8_t k = 0x80;
  uint8_t bit = 0;
  currentLed = 0;
  
  for(uint8_t ledNumber = 0; ledNumber < WS2812_LEDS_NUMBER; ledNumber++) {
    // each bit from wsLed[x] is extracted
    for (bit = 0; bit < 8; bit++) {
      txBuffer.append((bool) (wsLed[currentLed].green & (k >> bit)));
    }
    for (bit = 0; bit < 8; bit++) {
      txBuffer.append((bool) (wsLed[currentLed].red & (k >> bit)));
    }
    for (bit = 0; bit < 8; bit++) {
      txBuffer.append((bool) (wsLed[currentLed].blue & (k >> bit)));
    }
    currentLed++;
  }
}

void Leds::Clear(void) {
  uint16_t a[50];
  ws2812_diode_s d = {0,0,0};
  for(size_t i = 0; i < WS2812_LEDS_NUMBER; i++) wsLed[i] = d;
  memset(a, 0x00, 50);
  HAL_SPI_Transmit(&hspi2, (uint8_t*) a, 25, 20);
}

void Leds::Refresh(void) {
  ledsEnablePin.Set();
  // Start timer for 1s. Expiry disables dc/dc converter
  osTimerStart(ledTimeoutHandle, 1000);
  loadBuffer();
  Clear();
  HAL_SPI_Transmit_DMA(ledSpi, (uint8_t *)txBuffer.tx, sizeof(txBuffer.tx)/sizeof(txBuffer.tx[0]));
}

void Leds::Powerdown(void) { 
  //ledsEnablePin.Reset(); 
  // equivalent to above is
  HAL_GPIO_WritePin(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin, (GPIO_PinState) 0);
  int a = 0;
}

bool Leds::ShowBatteryState(uint8_t internalPercent, uint8_t externalPercent) {
  static uint8_t ovfCounter = 0; // Overflow counter
  static uint16_t i = 0;
  static bool countUp = true;
  uint8_t activeLeds = 0;
  ws2812_diode_s colour = {0,0,0};

  if(ovfCounter < 2) {
    // Internal battery animation
    activeLeds = convertPercentToLeds(internalPercent);
    //colour = {0x80, 0x00, 0xFF}; // violet
    colour.red = 0xFF;
    colour.green = 0x00;
    colour.blue = 0xFF;
  } else {
    // External battery animation
    activeLeds = convertPercentToLeds(externalPercent);
    colour = {0x00, 0xFF, 0xFF}; // yellow
  }

  uint8_t multiplier = gamma8(i) + 1;
  // Set each LED that is supposed to be active
  for(size_t led = 0; led < activeLeds; led++) {
    // Set each LED colour, but use sine value
    wsLed[led].red = colour.red & multiplier;
    wsLed[led].green = colour.green & multiplier;
    wsLed[led].blue = colour.blue & multiplier;
  }
  // i Runs from 0 to 0xFF back and forth
  // add/sub value determines speed of animation
  if(countUp) i+=3;
  else i-=3;

  if(i >= 0xFC) countUp = false;
  
  else if(i <= 1) {
    ovfCounter++;
    countUp = true;
  }
  
  // Internal and external flowed twice, end it
  if(ovfCounter >= 4) {
    i = 0;
    ovfCounter = 0;
    countUp = true;
    Clear();
    Refresh();
    Refresh();
    return true;
  }
  Refresh();
  return false;
}

uint8_t Leds::convertPercentToLeds(uint8_t percent) {
  if(percent < 20) return 1;
  else if(percent >= 20 && percent < 40) return 2;
  else if(percent >= 40 && percent < 60) return 3;
  else if(percent >= 60 && percent < 80) return 4;
  else if(percent >= 80 && percent <= 100) return 5;
  else if(percent > 100) return 5;
  else return 0;
}

bool Leds::IndicateRideModeChange(SS49::rideMode mode) {
  static uint8_t i = 0;
  static uint8_t ret = 0;
  ws2812_diode_s colour = {0,0,0};

  if(i++ > 10) {
    switch(mode) {
      case SS49::ECO: colour.green = 0x3F;
        break;
      case SS49::NORMAL: colour.blue = 0x3F;
        break;
      case SS49::BOOST: colour.red = 0x3F;
        break;
    }
    if(i > 20) {
      i = 0;
      ret++;
      return false;
    }
  }
  for(uint8_t led = 0; led < WS2812_LEDS_NUMBER; led++) {
      SetColour(colour, led);
  }
  Refresh();
  if(ret > 4) {
    ret = 0;
    Clear();
    Refresh();
    return true;
  }
  return false;
}

void Leds::IndicateLowBattery(Battery::BatteryType type) {
  static uint16_t i = 0;
  ws2812_diode_s colour;
  // internal - violet
  if(type == Battery::BatteryType::INTERNAL) colour = {0x10, 0, 0x08};
  //external - ocean blue
  else colour = {0, 0x10, 0x05};

  if(i++ > 400) {
    SetColour(colour, 0);
    if(i > 440) {
      Clear();
      i = 0;
    }
  }
  Refresh();
}

extern "C" void ledTimeoutCallback(void *argument) { 
  Leds::Powerdown();
  HAL_GPIO_WritePin(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin, GPIO_PIN_RESET);
};

uint8_t Leds::gamma8(uint8_t x) { return _gammaTable[x]; }
