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

//todo take care, data is inverted at v level converter

Leds::Leds() {
  //Logger::Log("WS2812 leds Constructor. \r\n");

  ledsEnablePin = Gpio(DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin);
  ledsEnablePin.Set();
  MX_SPI2_Init();
  ledSpi = &hspi2;
  // Clear tx buffer
  //Clear();
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
      txBuffer.append((bool) wsLed[currentLed].green & (k >> bit));
    }
    for (bit = 0; bit < 8; bit++) {
      txBuffer.append((bool) wsLed[currentLed].red & (k >> bit));
    }
    for (bit = 0; bit < 8; bit++) {
      txBuffer.append((bool) wsLed[currentLed].blue & (k >> bit));
    }
    currentLed++;
  }
}

void Leds::Clear(void) {
  memset(a, 0xff, 50);
  if(HAL_SPI_Transmit(&hspi2, (uint8_t*) a, 50, 20) != HAL_OK) {
    int b = 0;
  }
}

void Leds::Refresh(void) {
  ledsEnablePin.Set();

  Clear();
  loadBuffer();
  
  if(HAL_SPI_Transmit_DMA(ledSpi, (uint8_t *)txBuffer.tx, sizeof(txBuffer.tx)) != HAL_OK) {
    int a = 0;
  }
}

uint8_t Leds::GetCurrentLed(void) { return currentLed; }

void Leds::Powerdown(void) { 
  //ledsEnablePin.Reset(); 
}

bool Leds::ShowBatteryState(uint8_t internalPercent, uint8_t externalPercent) {
  static uint8_t ovfCounter; // Overflow counter
  static uint16_t i = 0;
  uint8_t activeLeds = 0;
  ws2812_diode_s colour = {0,0,0};
  bool counterFlag = true;

  if(ovfCounter < 2) {
    // Internal battery animation
    activeLeds = convertPercentToLeds(internalPercent);
    colour = {0x80, 0x00, 0xFF}; // violet
  } else {
    // External battery animation
    activeLeds = convertPercentToLeds(externalPercent);
    colour = {0x00, 0xFF, 0xFF}; // yellow
  }

  // Set each LED that is supposed to be active
  for(uint8_t led = 0; led < activeLeds; led++) {
    // Set each LED colour, but use sine value
    wsLed[led].red = colour.red * gamma8(i);
    wsLed[led].green = colour.green * gamma8(i);
    wsLed[led].blue = colour.blue * gamma8(i);
  }

  // i Runs from 0 to 0xFF back and forth
  // add/sub value determines speed of animation
  if(counterFlag) i+=2;
  else i-=2;

  if(i >= 0xFF) {
    counterFlag = false;
    ovfCounter++;
  }
  else if(i <= 0) counterFlag = true;

  Refresh();

  // Internal and external flowed twice, end it
  return ovfCounter >= 4 ? true : false;
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

extern "C" {

// void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
//   if (hspi == &hspi2) {
//   }
// }

void ledTimeoutCallback(void *argument) { 
  /*ledsPointer->Powerdown();*/ 
};

}  // extern C close



uint8_t Leds::sine8(uint8_t x) { return _sineTable[x]; }

uint8_t Leds::gamma8(uint8_t x) { return _gammaTable[x]; }
