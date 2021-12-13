/*
 * radioApp.hpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#ifndef __RADIOAPP_HPP
#define __RADIOAPP_HPP

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "spi.h"
#include "gpioApp.hpp"

struct RadioConfig
{
    // Interval at which radio task is being run
  const uint16_t taskTimeInterval = 20; //[ms]

  const uint8_t payloadSize = 16;

    // Radio transmit channel
  const uint8_t channel = 52;

    // Pipe addreses
  const uint64_t txPipeAddress = 0xCAFEBABE;
  const uint64_t rxPipeAddress = 0xDEADBEEF;

    // Used gpio pins
  GPIO_TypeDef * port = NRF24_CE_GPIO_Port;
  const uint16_t cePin = NRF24_CE_Pin;
  const uint16_t csnPin = NRF24_CSN_Pin;
};

class NRF24 {
 public: const RadioConfig config;

 private:
    // Safety flag for changing power mode
  bool isSleeping;
    // Pins only initalised. State set by HAL in NRF24 drivers
  Gpio pinCE = Gpio(config.port, config.cePin);
  Gpio pinCSN = Gpio(config.port, config.csnPin);

 public:
  NRF24(SPI_HandleTypeDef * hspi);

  /**
   * Transmit given payload, constant length
   * @param payload address
   * @return success(1) / fail(0)
   */
  bool Write(void *payload);

  /**
   * Check if any payload has been recieved by radio
   */
  bool IsAvailable(void);

  /**
   * Read recieved payload
   * @param data address
   */
  void Read(void *data);

  /**
   * Put radio into sleep mode
   */
  void Sleep(void);

  /**
   * Power up the radio. Contains blocking delay of 1.5 ms
   */
  void Wakeup(void);

  uint8_t IsInitialised(void);
};

#endif /* __RADIOAPP_HPP */