/*
 * radioApp.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#include "radioApp.hpp"
#include "logger.hpp"

#define RADIO_TASK_TIME_INTERVAL 5000

#define RADIO_WRITE_PIPE 0xCAFEBABE
#define RADIO_CHANNEL 52
#define RADIO_PAYLOAD_SIZE 32
#define RADIO_AUTO_ACK 1

#define NRF24_CSN_PIN 0
#define NRF24_CE_PIN 0

extern SPI_HandleTypeDef hspi1;

#ifdef __cplusplus
extern "C" {
#endif

void RadioTask(void const *argument)
{
	uint32_t taskTimer = (uint32_t)osKernelGetTickCount + RADIO_TASK_TIME_INTERVAL;
	NRF24 radio;
	void * rxData;

	for (;;)
	{
		if (radio.IsAvailable())
		{
			radio.Read(rxData);
		}
		radio.Write((void *)"x");

		vTaskDelay(taskTimer);
	}
}

#ifdef __cplusplus 
}
#endif

NRF24::NRF24()
{
	NRF24_begin(GPIOB, NRF24_CSN_PIN, NRF24_CE_PIN, hspi1);

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
	return NRF24_write(payload, RADIO_PAYLOAD_SIZE);
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