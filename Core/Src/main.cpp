/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "task.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
//#include "usb.h"
#include "gpio.h"

extern "C" {
/* Private function prototypes * -----------------------------------------------*/
void MX_FREERTOS_Init(void);

} // extern C close

void SystemClock_Config(void);

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
  MX_DMA_Init();
  //MX_ADC_Init();
  //todo init those in specific modules inits
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  //MX_USB_PCD_Init();

  for (int i = 0; i < 10; i++)
  {
    HAL_GPIO_TogglePin(LED_USER_GPIO_Port, LED_USER_Pin);
    HAL_Delay(50);
  }

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  while (1){ 

  }
}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void) 
{ 
RCC_OscInitTypeDef RCC_OscInitStruct; 
RCC_ClkInitTypeDef RCC_ClkInitStruct;

/** Configure the main internal regulator output voltage 
*/ 
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); 
/** Initializes the RCC Oscillators according to the specified parameters 
* in the RCC_OscInitTypeDef structure. 
*/ 
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; 
RCC_OscInitStruct.HSIState = RCC_HSI_ON; 
RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; 
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; 
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI; 
RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6; 
RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3; 
if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) 
{ 
  Error_Handler(); 
} 
/** Initializes the CPU, AHB and APB buses clocks 
*/ 
RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK 
                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2; 
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; 
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; 
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; 
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; 

if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) 
{ 
  Error_Handler(); 
} 
} 

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
* @brief  Period elapsed callback in non blocking mode
* @note   This function is called  when TIM6 interrupt took place, inside
* HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
* a global variable "uwTick" used as application time base.
* @param  htim : TIM handle
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
