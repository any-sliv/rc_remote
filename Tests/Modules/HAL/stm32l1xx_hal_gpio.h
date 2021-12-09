#ifndef __STM32L1xx_HAL_GPIO_H
#define __STM32L1xx_HAL_GPIO_H

#include "stdint.h"
#include "stm32l1xx_hal_def.h"

#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT_PP 0
#define __HAL_RCC_GPIOC_CLK_ENABLE() dummy()
#define __HAL_RCC_GPIOB_CLK_ENABLE() dummy()
#define __HAL_RCC_GPIOA_CLK_ENABLE() dummy()

void dummy(void);

typedef struct
{
  uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins */

  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref GPIO_mode */

  uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref GPIO_pull */

  uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                           This parameter can be a value of @ref GPIO_speed */

  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins
                            This parameter can be a value of @ref GPIOEx_Alternate_function_selection */
} GPIO_InitTypeDef;

typedef struct
{
  uint32_t MODER;        /*!< GPIO port mode register,      */
  uint32_t OTYPER;       /*!< GPIO port output type register,      */
  uint32_t IDR;          /*!< GPIO port input data register, */              
  uint32_t ODR;          /*!< GPIO port output data register,  */          
  uint32_t BSRR;         /*!< GPIO port bit set/reset registerBSRR, */
} GPIO_TypeDef;

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

#define  GPIO_NOPULL        (0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        (0x00000001U)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      (0x00000002U)   /*!< Pull-down activation                */

#define GPIO_SPEED_FREQ_LOW 0 

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);

#endif //------------------------------------