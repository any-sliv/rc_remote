/*
 * radioApp.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#include "FreeRTOS.h"
#include "timers.h"
#include "radioApp.hpp"
#include "logger.hpp"
extern "C" {
  #include "nrf24.h"
} // extern C close

#define RADIO_TASK_TIME_INTERVAL 20
#define RADIO_RECEIVE_TIMEOUT 5000

#define RADIO_CHANNEL 52
#define RADIO_PAYLOAD_SIZE 32
#define RADIO_AUTO_ACK 1

#ifdef __cplusplus
extern "C" {
#endif

extern SPI_HandleTypeDef hspi1;
// HAL UART handle
extern UART_HandleTypeDef huart1;
// OS managed timer
extern osTimerId_t radioHeartbeatHandle;

uint64_t txPipe = 0xCAFEBABE;

void RadioTask(void * argument) {
  uint32_t taskTimer = RADIO_TASK_TIME_INTERVAL;
  Gpio pin = Gpio(LED_USER_GPIO_Port, LED_USER_Pin);
  NRF24 radio = NRF24(&hspi1);
  void *rxData;
  bool flag = false;

  char data[32] = "hei";

  for (;;) {
    // if (radio.IsAvailable()) {
    //   radio.Read(rxData);
    //   osTimerStop(radioHeartbeatHandle);
    // } else {
    //   // Run timer if not already running
    //   if (!(osTimerIsRunning(radioHeartbeatHandle))) {
    //     osTimerStart(radioHeartbeatHandle, RADIO_TASK_TIME_INTERVAL);
    //   }
    // }

    if(NRF24_write(&data, 32)) {
      //Logger::Log("DataSentSuccessfully \r\n", 32);
      pin.Set();
    } else {
      Logger::Log("CannotSendSata \r\n", 32);
      radio.Sleep();
      radio.Wakeup();
      pin.Reset();
    }

    // flag ^= 1;
    // pin.Set(flag);

    vTaskDelay(taskTimer);
  }  // -------------------------------------------------------------------------
}

extern void radioHeartbeatCallback(void *argument) {
  // radio.Sleep();
  NRF24_powerDown();
}

#ifdef __cplusplus
}
#endif

NRF24::NRF24(SPI_HandleTypeDef * hspi) {
  NRF24_begin(NRF24_CE_GPIO_Port, NRF24_CE_Pin, NRF24_CSN_Pin, hspi);

  NRF24_stopListening();
  NRF24_openWritingPipe(txPipe);
  //NRF24_setAutoAck(RADIO_AUTO_ACK);
  NRF24_setChannel(50);
  NRF24_setPayloadSize(32);
  
//  NRF24_enableDynamicPayloads();
//  NRF24_enableAckPayload();
}

bool NRF24::Write(void *payload) {
  NRF24_stopListening();
  NRF24_openWritingPipe(txPipe);
  bool ret = NRF24_write(payload, RADIO_PAYLOAD_SIZE);
  NRF24_openReadingPipe(1, 0xCAFEBABE);
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
    //osDelay(2);
    isSleeping = false;
  }
}

uint8_t NRF24::IsInitialised(void) {
  return NRF24_get_status();
}

// --------------------------------------------------------