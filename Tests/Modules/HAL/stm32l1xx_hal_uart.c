#include "stm32l1xx_hal_uart.h"

uint8_t * data;

// HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, 
//                                     uint8_t *pData, uint16_t Size, uint32_t Timeout) {

// }

uint8_t * GetDataAddress(void) {
  return data;
}

HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, 
                                    uint8_t *pData, uint16_t Size) {
  data = pData;
  return HAL_OK;
}