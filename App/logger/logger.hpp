/*
 * logger.hpp
 *
 *  Created on: Sep 19, 2021
 *      Author: macsli
 */

#ifndef __LOGGER_HPP
#define __LOGGER_HPP

#include <string>

extern "C" {
#include "stm32l1xx_hal.h"
} // extern C close

void Log(std::string text);

class Logger {
 public:
  Logger();

  /**
   * Give me your text and I log it
   * @param text string to be printed in UART interface
  */
  static void Log(std::string text);

  static void Log(char * data);
};



#endif /* __LOGGER_HPP */
