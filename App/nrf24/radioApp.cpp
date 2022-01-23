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
  #include "semphr.h"
} // extern C close

extern "C" {
  extern SPI_HandleTypeDef hspi1;
  extern UART_HandleTypeDef huart1;
  extern osTimerId_t radioHeartbeatHandle;
} // extern C close

extern "C" void RadioTask(void * argument) {
  NRF24 radio = NRF24(&hspi1);
  Gpio pin = Gpio(LED_USER_GPIO_Port, LED_USER_Pin);
  bool flag = false;

  osTimerStart(radioHeartbeatHandle, 5000);

  for (;;) {
    // Set throttle data as data to transmit
    int rcv = 0;
    xQueueReceive(qRadioTxValueHandle, &rcv, 0);
    if(radio.Write(rcv)) {
      int read = radio.Read(radio.rxData);
      xQueueSend(qBatteryExternalHandle, &read, 0);
      flag ^= 1;
      pin.Set(flag);
    } else {
      pin.Reset();
    }
    // Active heartbeat if data is sent
    if(rcv) osTimerStart(radioHeartbeatHandle, 5000);

    flag ^= 1;
    pin.Set(flag);

    xSemaphoreGive(radioTxDoneSemaphoreHandle);
    vTaskDelay(radio.config.taskTimeInterval);
  }  // -------------------------------------------------------------------------
}

extern "C" void radioHeartbeatCallback(void *argument) {
  NRF24::Sleep();
}

NRF24::NRF24(SPI_HandleTypeDef * hspi) {
  Logger::Log("NRF24 radio Constructor. \r\n");
  Wakeup();

  memset(rxData, 0, sizeof(rxData));
  MX_SPI1_Init();

  NRF24_begin(config.port, config.csnPin, config.cePin, hspi);
  NRF24_stopListening();
  NRF24_openWritingPipe(config.txPipeAddress);
  NRF24_setAutoAck(1);
  NRF24_setChannel(config.channel);
  NRF24_setPayloadSize(config.payloadSize);

  // ! WARNING: enabling this function messes with RT OS. Blocks other tasks.
  // ! USE ONLY WHEN NECESSARY
  
  //printRadioSettings();
  
  NRF24_enableAckPayload();
  NRF24_enableDynamicPayloads();

  NRF24_startListening();
}

bool NRF24::Write(int data) {
  sprintf((char *)txData, "THR:%d \r\n", data);

  //NRF24_stopListening();

  bool ret = NRF24_write(&txData, config.payloadSize);

  // Clear tx buffer
  memset(&txData, 0, sizeof(rxData));

  return ret;
}

bool NRF24::IsAvailable(void) { return NRF24_available(); }

int NRF24::Read(void *data) { 
  NRF24_read(data, config.payloadSize); 
  uint8_t rx[32];
  memset(rx, 0, sizeof(rx));
  memcpy(rx, data + 4, 16);
  return atoi((const char *) rx);
}

void NRF24::Sleep(void) {
  //NRF24_stopListening();
  NRF24_powerDown();
  osDelay(1);
}

void NRF24::Wakeup(void) {
  NRF24_powerUp();
  // Powerup requires 1.5ms run up delay
  osDelay(2);
}

uint8_t NRF24::IsInitialised(void) {
  return NRF24_get_status();
}

// --------------------------------------------------------
