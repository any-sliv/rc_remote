/*
 * radioApp.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#include "radioApp.hpp"
#include "logger.hpp"
#include "timers.h"

#define RADIO_TASK_TIME_INTERVAL 	5
#define RADIO_RECEIVE_TIMEOUT		5000

#define RADIO_WRITE_PIPE 	0xCAFEBABE
#define RADIO_CHANNEL 		52
#define RADIO_PAYLOAD_SIZE 	32
#define RADIO_AUTO_ACK	 	1

// HAL SPI handle
extern SPI_HandleTypeDef hspi1;
// OS managed timer
extern osTimerId_t radioHeartbeatHandle;

uint32_t taskTimer = (uint32_t)osKernelGetTickCount + RADIO_TASK_TIME_INTERVAL;
NRF24 radio;
void * rxData;

#ifdef __cplusplus
extern "C" {
#endif

void RadioTask(void const *argument)
{
	for (;;) {
		if (radio.IsAvailable()) {
			radio.Read(rxData);
			osTimerStop(radioHeartbeatHandle);
		} else {
			// Run timer if not already running
			if(!(osTimerIsRunning(radioHeartbeatHandle))) {
				osTimerStart(radioHeartbeatHandle, RADIO_TASK_TIME_INTERVAL);
			}
		}
		
		radio.Write((void *)"x");

		vTaskDelay(taskTimer);
	} // -------------------------------------------------------------------------
}

extern void radioHeartbeatCallback(void *argument)
{
	Logger::LogDebug("Radio no data recieved");
	Logger::LogDebug("Entering sleep (zzz...)");

	//radio.Sleep();
	NRF24_powerDown();
}

#ifdef __cplusplus 
}
#endif

NRF24::NRF24()
{
	NRF24_begin(GPIOB, NRF24_CSN_Pin, NRF24_CE_Pin, hspi1);

	//printRadioSettings();

	NRF24_stopListening();
	NRF24_openWritingPipe(RADIO_WRITE_PIPE);
	NRF24_setAutoAck(RADIO_AUTO_ACK);
	NRF24_setChannel(RADIO_CHANNEL);
	NRF24_setPayloadSize(RADIO_PAYLOAD_SIZE);

	NRF24_enableDynamicPayloads();
	NRF24_enableAckPayload();
}

bool NRF24::Write(void *payload)
{
	NRF24_stopListening();
	bool ret = NRF24_write(payload, RADIO_PAYLOAD_SIZE);
	NRF24_startListening();
	return ret;
}

bool NRF24::IsAvailable(void)
{
	return NRF24_available();
}

void NRF24::Read(void *data)
{
	NRF24_read(data, RADIO_PAYLOAD_SIZE);
}

void NRF24::Sleep(void)
{
	NRF24_powerDown();
	isSleeping = true;
}

void NRF24::Wakeup(void)
{
	if (isSleeping)
	{
		NRF24_powerUp();
		// Powerup requires 1.5ms run up delay
		HAL_Delay(1500);
		isSleeping = false;
	}
}

// --------------------------------------------------------