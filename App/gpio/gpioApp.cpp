/*
 * gpio.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: macsli
 */

#ifdef __cplusplus 
extern "C" { 
#endif 

#include "gpioApp.hpp"


Gpio::Gpio(GPIO_TypeDef *_port, uint32_t _pin, 
            bool initState, uint32_t mode,
      uint32_t pull) {
  pin = _pin;
  port = _port;

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = pull;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(port, &GPIO_InitStruct);

  Set(initState);
}

void Gpio::Set(void) { 
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)1U); 
}

void Gpio::Set(bool value) {
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)value);
}

void Gpio::Reset(void) { 
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)0); 
}

bool Gpio::Read(void) {
  return (bool)HAL_GPIO_ReadPin(port, pin);
}

uint32_t Gpio::GetPinNumber(void) {
  return pin;
}

#ifdef __cplusplus 
} 
#endif 