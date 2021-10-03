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

extern UART_HandleTypeDef huart1;


bool Logger::ILogDebug(std::string textString) {
    uint8_t * txBuffer = reinterpret_cast<uint8_t *> (textString[0]);
    HAL_UART_Transmit_IT(&huart1, txBuffer, textString.length());

    return 0;
}

Logger& Logger::Get() {
    return logInstance;
}

bool Logger::LogDebug(std::string text) {
    return Get().ILogDebug(text);
}
