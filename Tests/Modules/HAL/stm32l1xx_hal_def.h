#ifndef __STM32L1xx_HAL_DEF_H
#define __STM32L1xx_HAL_DEF_H

#include "stdint.h"

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#endif