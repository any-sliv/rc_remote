/*
 * ss49.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */
extern "C" {
#include "stm32l1xx_hal.h"
}  // extern C close

#include "analogRead.hpp"

class SS49 : AnalogRead {
 private:
  ADC_ChannelConfTypeDef channelConfig = {
    .Channel = 0,
    .Rank = 1,
    .SamplingTime = ADC_SAMPLETIME_48CYCLES
  };

  // todo try to make it constant and assign value after init
  int neutralThrottle = 0;

 public:
  /**
   * Initiate the sensor.
   * @param channel configuration
   */
  SS49(ADC_ChannelConfTypeDef *channel);
  /**
   * If no channel given as a param use 
   * default config of class member @param channelConfig 
   */
  SS49(void);

  /**
   * Blocking function
   * @return Value of average throttle position measured
   * @param samplingPeriod period of samples gathering [ms]
   * in time given by <this>.samplingPeriod * 1
   * Calibration value is saved in variable "neutralThrottle"
   */
  void CalibrateInitialPosition(uint32_t samplingPeriod);

  /**
   * Return position of hall sensor.
   * @return 0 - neutral, negative values when braking, positive when
   * accelerating
   */
  int GetPosition(void);
};