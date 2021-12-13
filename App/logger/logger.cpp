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

// todo create a queue for logger messages and
// todo post them in some task periodically

// bool Logger::ILogDebug(std::string textString, va_list args) {
//   uint8_t* txBuffer = reinterpret_cast<uint8_t*>(textString[0]);
//   HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, txBuffer, textString.length() + 1, 5);

//   // HAL_OK is 0 -> returns true.
//   return ret ? false : true;
// }

// Logger& Logger::Get() { 
//   return logInstance; 
// }

// bool Logger::LogDebug(std::string text, ...) {
//   va_list args;
//   va_start(args, text);

//   bool ret = Get().ILogDebug(text, args);
//   va_end(args);
//   return ret;
// }

void Logger::Log(char * data, uint16_t len) {
  // text.append("\r\n");
  // const char *cstr = text.c_str();
  HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)data, len + 1, 5);
}