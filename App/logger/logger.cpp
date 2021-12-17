/*
 * logger.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#include "logger.hpp"
#include <cstdarg>
#include "usart.h"

extern "C" {
  extern UART_HandleTypeDef huart1;
} // extern C close

// todo create a queue for logger messages and
// todo post them in some task periodically


void Logger::Log(std::string text) {
  // va_list args;
  // va_start(args, text);
  //todo use va_list
  text.append("\r\n");
  const char * txBuffer = text.c_str();
  HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)txBuffer, text.length() + 1, 5);
}

void Logger::Log(char * data, uint16_t len) {
  // text.append("\r\n");
  // const char *cstr = text.c_str();
  HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)data, len + 1, 5);
}