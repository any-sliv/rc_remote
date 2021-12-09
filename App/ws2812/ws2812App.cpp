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

// Global variables only for callback functions
extern osTimerId_t ledTimeoutHandle;
Leds *ledsPointer = NULL;

Leds::Leds() {
  ledsPointer = this;

  ledsEnablePin = new Gpio{DCDC_ENABLE_GPIO_Port, DCDC_ENABLE_Pin};

  MX_SPI2_Init();
  extern SPI_HandleTypeDef hspi2;
  ledSpi = &hspi2;

  Logger::LogDebug("Leds Init");
}

Leds::~Leds() { HAL_SPI_MspDeInit(ledSpi); }

void Leds::SetColour(ws2812_diode_s wsStruct, uint8_t ledNumber) {
  if (ledNumber > sizeof(wsLed)) return;
  wsLed[ledNumber] = wsStruct;
}

uint16_t *Leds::loadBuffer(void) {
  const uint8_t k = 0x80;
  uint8_t bit = 0;

  buffer.flip();

  // each bit from wsLed[x] is extracted
  for (bit = 0; bit < 8; bit++) {
    buffer.append(wsLed[currentLed].green && (k >> bit));
  }
  for (bit = 0; bit < 8; bit++) {
    buffer.append(wsLed[currentLed].red && (k >> bit));
  }
  for (bit = 0; bit < 8; bit++) {
    buffer.append(wsLed[currentLed].blue && (k >> bit));
  }

  if (!(currentLed >= sizeof(wsLed))) {
    currentLed++;
  }

  return buffer.active();
}

void Leds::Clear(void) {
  for (auto &&i : wsLed) {
    i.red = 0;
    i.green = 0;
    i.blue = 0;
  }
}

void Leds::Refresh(void) {
  ledsEnablePin->Set();
  // Run a timer which expiry disables leds power supply
  osTimerStart(ledTimeoutHandle, timeout);
  loadBuffer();
  HAL_SPI_Transmit_DMA(ledSpi, (uint8_t *)buffer.active(), 24);
}

uint8_t Leds::GetCurrentLed(void) { return currentLed; }

void Leds::Powerdown(void) { ledsEnablePin->Reset(); }

extern "C" {

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi) {
  if (hspi == &hspi2) {
    if (ledsPointer->GetCurrentLed() > WS2812_LEDS_NUMBER) {
      HAL_SPI_DMAStop(hspi);
      return;
    }

    uint16_t *bufferPointer = ledsPointer->loadBuffer();
    HAL_SPI_Transmit_DMA(hspi, (uint8_t *)bufferPointer, 24);
  }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  if (hspi == &hspi2) {
  }
}

void ledTimeoutCallback(void *argument) { ledsPointer->Powerdown(); }

}  // extern C close

static const uint8_t _sineTable[256] = {
    0x40, 0x41, 0x43, 0x44, 0x46, 0x47, 0x49, 0x4a, 0x4c, 0x4d, 0x4f, 0x50,
    0x52, 0x53, 0x55, 0x56, 0x58, 0x59, 0x5b, 0x5c, 0x5d, 0x5f, 0x60, 0x61,
    0x63, 0x64, 0x65, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6f, 0x70,
    0x71, 0x72, 0x73, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x78, 0x79, 0x7a,
    0x7a, 0x7b, 0x7b, 0x7c, 0x7c, 0x7d, 0x7d, 0x7d, 0x7e, 0x7e, 0x7e, 0x7f,
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 0x7e,
    0x7e, 0x7d, 0x7d, 0x7d, 0x7c, 0x7c, 0x7b, 0x7b, 0x7a, 0x7a, 0x79, 0x78,
    0x78, 0x77, 0x76, 0x75, 0x74, 0x73, 0x73, 0x72, 0x71, 0x70, 0x6f, 0x6d,
    0x6c, 0x6b, 0x6a, 0x69, 0x68, 0x67, 0x65, 0x64, 0x63, 0x61, 0x60, 0x5f,
    0x5d, 0x5c, 0x5b, 0x59, 0x58, 0x56, 0x55, 0x53, 0x52, 0x50, 0x4f, 0x4d,
    0x4c, 0x4a, 0x49, 0x47, 0x46, 0x44, 0x43, 0x41, 0x40, 0x3e, 0x3c, 0x3b,
    0x39, 0x38, 0x36, 0x35, 0x33, 0x32, 0x30, 0x2f, 0x2d, 0x2c, 0x2a, 0x29,
    0x27, 0x26, 0x24, 0x23, 0x22, 0x20, 0x1f, 0x1e, 0x1c, 0x1b, 0x1a, 0x18,
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x10, 0xf,  0xe,  0xd,  0xc,  0xc,
    0xb,  0xa,  0x9,  0x8,  0x7,  0x7,  0x6,  0x5,  0x5,  0x4,  0x4,  0x3,
    0x3,  0x2,  0x2,  0x2,  0x1,  0x1,  0x1,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x1,  0x1,  0x2,  0x2,  0x2,
    0x3,  0x3,  0x4,  0x4,  0x5,  0x5,  0x6,  0x7,  0x7,  0x8,  0x9,  0xa,
    0xb,  0xc,  0xc,  0xd,  0xe,  0xf,  0x10, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x1a, 0x1b, 0x1c, 0x1e, 0x1f, 0x20, 0x22, 0x23, 0x24, 0x26,
    0x27, 0x29, 0x2a, 0x2c, 0x2d, 0x2f, 0x30, 0x32, 0x33, 0x35, 0x36, 0x38};

static const uint8_t _gammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

uint8_t sine8(uint8_t x) { return _sineTable[x]; }

uint8_t gamma8(uint8_t x) { return _gammaTable[x]; }