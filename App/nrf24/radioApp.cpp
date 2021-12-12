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
#include "gpioApp.hpp"
extern "C" {
  #include "nrf24.h"
} // extern C close

#define RADIO_TASK_TIME_INTERVAL 500
#define RADIO_RECEIVE_TIMEOUT 5000

#define RADIO_WRITE_PIPE 0xCAFEBABE
#define RADIO_CHANNEL 52
#define RADIO_PAYLOAD_SIZE 32
#define RADIO_AUTO_ACK 1

#ifdef __cplusplus
extern "C" {
#endif

// HAL UART handle
extern UART_HandleTypeDef huart1;
// OS managed timer
extern osTimerId_t radioHeartbeatHandle;

void RadioTask(void * argument) {
  SPI_HandleTypeDef * radioSPI = GetSpiHandle(1);
  uint32_t taskTimer = RADIO_TASK_TIME_INTERVAL;
  Gpio pin = Gpio(LED_USER_GPIO_Port, LED_USER_Pin);
  NRF24 radio = NRF24(radioSPI);
  void *rxData;
  bool flag = false;

  //if(radio.IsInitialised()) pin.Set();

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

    // if(radio.Write((void *)"x")) {
    //   Logger::LogDebug("Data sent successfully");
    // } else {
    //   Logger::LogDebug("Cannot send data");
    // }

    // if(radio.IsAvailable()) {
    //   Logger::Log("Radio available");
    // }
    flag ^= 1;
    pin.Set(flag);

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
  NRF24_begin(NRF24_CE_GPIO_Port, NRF24_CSN_Pin, NRF24_CE_Pin, hspi);

  // printRadioSettings();

  NRF24_stopListening();
  NRF24_openWritingPipe(RADIO_WRITE_PIPE);
//  NRF24_setAutoAck(RADIO_AUTO_ACK);
  NRF24_setChannel(RADIO_CHANNEL);
  NRF24_setPayloadSize(RADIO_PAYLOAD_SIZE);

//  NRF24_enableDynamicPayloads();
//  NRF24_enableAckPayload();
}

bool NRF24::Write(void *payload) {
  NRF24_stopListening();
  bool ret = NRF24_write(payload, RADIO_PAYLOAD_SIZE);
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
    osDelay(1500);
    isSleeping = false;
  }
}

uint8_t NRF24::IsInitialised(void) {
  return NRF24_get_status();
}

// --------------------------------------------------------