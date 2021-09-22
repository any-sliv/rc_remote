/*
 * logger.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#include "logger.hpp"
#include "usart.h"
#include <string>

extern UART_HandleTypeDef huart1;


// Singleton class
class Logger {
public:
    static Logger& Get() {
        return logInstance;
    }
    
    static bool LogDebug(std::string text) {
        return Get().ILogDebug(text);
    }

private:
    // Dummy constructor
    Logger();

    static Logger logInstance;

    bool ILogDebug(std::string textString) {
        uint8_t * txBuffer = reinterpret_cast<uint8_t *> (textString[0]);

        HAL_UART_Transmit_IT(&huart1, txBuffer, textString.length());

        return 0;
    }
};