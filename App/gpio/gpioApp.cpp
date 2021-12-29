/*
 * gpioApp.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: macsli
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "gpioApp.hpp"

Gpio::Gpio(GPIO_TypeDef *_port, uint32_t _pin, bool initState, uint32_t mode,
           uint32_t pull) {

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  pin = _pin;
  port = _port;

  GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin = pin,
    .Mode = mode,
    .Pull = pull,
    };
  
  if(mode == OUTPUT) {
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  }

  HAL_GPIO_Init(port, &GPIO_InitStruct);

  //Set(initState);
}

Gpio::Gpio() {};

void Gpio::Set(void) { HAL_GPIO_WritePin(port, pin, (GPIO_PinState)1U); }

void Gpio::Set(bool value) {
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)value);
}

void Gpio::Reset(void) { HAL_GPIO_WritePin(port, pin, (GPIO_PinState)0); }

bool Gpio::Read(void) { return (bool)HAL_GPIO_ReadPin(port, pin); }

uint32_t Gpio::GetPinNumber(void) { return pin; }

//TODO deconstructor

#ifdef __cplusplus
}
#endif