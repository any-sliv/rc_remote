/*
 * radioApp.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include "radioApp.hpp"
#include "logger.hpp"
#include "buttonApp.hpp"
extern "C" {
  #include "nrf24.h"
} // extern C close

#define RADIO_CHANNEL 52
#define RADIO_PAYLOAD_SIZE 32
#define RADIO_AUTO_ACK 1

extern "C" {
  extern SPI_HandleTypeDef hspi1;
  extern UART_HandleTypeDef huart1;
  extern osTimerId_t radioHeartbeatHandle;
} // extern C close

extern "C" void RadioTask(void * argument) {
  NRF24 radio = NRF24(&hspi1);
  Gpio pin = Gpio(LED_USER_GPIO_Port, LED_USER_Pin);
  bool flag = false;
  //void * rxData;
  //uint8_t * txData = reinterpret_cast<uint8_t *>(malloc(radio.config.payloadSize));
  //uint8_t * txData = new uint8_t(radio.config.payloadSize);

  for (;;) {
    // if (radio.IsAvailable()) {
    //   radio.Read(rxData);
    // } else {

    // }

    // xQueueReceive(qRadioTxValueHandle, rxData, 0);
    // if(radio.Write(rxData)) {
    //   flag ^= 1;
    //   pin.Set(flag);
    // }
    //   //Logger::Log("DataSentSuccessfully \r\n", 32);
    //   pin.Set();
    // } else {
    //   Logger::Log("CannotSendSata \r\n", radio.config.payloadSize);
    //   pin.Reset();
    // }
    flag ^= 1;
    pin.Set(flag);

    vTaskDelay(radio.config.taskTimeInterval);
  }  // -------------------------------------------------------------------------
}

extern "C" void radioHeartbeatCallback(void *argument) {
  // radio.Sleep();
}

NRF24::NRF24(SPI_HandleTypeDef * hspi) {
  Logger::Log("NRF24 radio Constructor.");
  NRF24_begin(config.port, config.cePin, config.csnPin, hspi);

  NRF24_stopListening();
  NRF24_openWritingPipe(config.txPipeAddress);
  //NRF24_setAutoAck(RADIO_AUTO_ACK);
  NRF24_setChannel(config.channel);
  NRF24_setPayloadSize(config.payloadSize);

  // WARNING: enabling this function messes with RT OS. Blocks other tasks.
  // USE ONLY WHEN NECESSARY
  //printRadioSettings();
  
 //NRF24_enableDynamicPayloads();
 //NRF24_enableAckPayload();
}

bool NRF24::Write(void *payload) {
  NRF24_stopListening();
  NRF24_openWritingPipe(config.txPipeAddress);

  bool ret = NRF24_write(payload, config.payloadSize);

  NRF24_openReadingPipe(1, config.rxPipeAddress);
  NRF24_startListening();
  return ret;
}

bool NRF24::IsAvailable(void) { return NRF24_available(); }

void NRF24::Read(void *data) { NRF24_read(data, RADIO_PAYLOAD_SIZE); }

void NRF24::Sleep(void) {
  NRF24_powerDown();
  isSleeping = true;
}

void NRF24::Wakeup(void) {
  if (isSleeping) {
    NRF24_powerUp();
    // Powerup requires 1.5ms run up delay
    osDelay(2);
    isSleeping = false;
  }
}

uint8_t NRF24::IsInitialised(void) {
  return NRF24_get_status();
}

// --------------------------------------------------------
