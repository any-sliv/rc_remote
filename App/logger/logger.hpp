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

class Logger {
 private:
  Logger();

 public:

  static void Log(std::string text);

  static void Log(char * data, uint16_t len);
  /**
   * Give me your text and I log it
   * @param text string to be printed in UART interface
   * @return success(1) / fail(0)
   */
  static bool LogDebug(std::string text, ...);
};

#endif /* __LOGGER_HPP */
