#include "stm32l1xx_hal_gpio.h"

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  if (PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin;
  }
  else
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16 ;
  }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin) {
  return GPIO_PIN_RESET;
};

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init) {};