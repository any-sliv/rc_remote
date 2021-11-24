extern "C" {
  #include "stm32l1xx_hal_gpio.h"
} // extern C close

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  GPIOx->ODR = (PinState << GPIO_Pin);
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin) {
  return (GPIO_PinState)0;
};

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init) {};