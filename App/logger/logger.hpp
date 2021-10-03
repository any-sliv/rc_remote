/*
 * logger.hpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#include <string>

class Logger {
private:
    Logger();
    static Logger logInstance;
    static bool ILogDebug(std::string textString);

public: 
    static Logger& Get();
    static bool LogDebug(std::string text);

};
