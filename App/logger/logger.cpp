/*
 * logger.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#include "stm32l1xx_hal.h"
#include "logger.hpp"
#include "usart.h"
#include <string>
#include <cstdarg>

extern UART_HandleTypeDef huart1;


bool Logger::ILogDebug(std::string textString, va_list args) {
    uint8_t * txBuffer = reinterpret_cast<uint8_t *> (textString[0]);
    HAL_UART_Transmit_IT(&huart1, txBuffer, textString.length());

    return 1; //TODO
}

Logger& Logger::Get() {
    return logInstance;
}


bool Logger::LogDebug(std::string text, ...) {
    va_list args;
    va_start(args, text);
    
    bool ret = Get().ILogDebug(text, args);
    va_end(args);
    return ret;
}
