/*
 * logger.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#include "logger.hpp"
#include <cstdarg>

extern "C" {
  #include "usart.h"
  extern UART_HandleTypeDef huart1;
} // extern C close

Logger::Logger() {};

void Logger::Log(char * data) {
  HAL_UART_Transmit(&huart1, (uint8_t *)data, 32, 5);
}