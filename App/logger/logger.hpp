/*
 * logger.hpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <string>

class Logger {

private:

    Logger();
    static Logger logInstance;
    static bool ILogDebug(std::string textString, va_list args);

public: 
    static Logger& Get();

    /**
     * Give me your text and I log it
     * @param text string to be printed in UART interface
     * @return success(1) / fail(0)
     */
    static bool LogDebug(std::string text, ...);

};

#endif /* INC_LOGGER_H_ */
