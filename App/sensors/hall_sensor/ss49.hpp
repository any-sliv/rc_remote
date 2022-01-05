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
 public: enum rideMode {
    NORMAL = 0,
    ECO = 1,
    BOOST = 2
  };
  
 private:
  ADC_ChannelConfTypeDef channelConfig = {
    .Channel = 0,
    .Rank = 1,
    .SamplingTime = ADC_SAMPLETIME_48CYCLES
  };

  uint32_t samplingPeriod = 200;

  int neutralThrottle = 0;

    // Ride mode
  rideMode mode = NORMAL;

 public:
  /**
   * Initiate the sensor. Used in unit test scenario.
   * @param channel configuration
   */
  SS49(ADC_ChannelConfTypeDef *channel);
  /**
   * If no channel given as a param use
   * default config of class member @param channelConfig
   * Runs calibration - blocking function.
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
  
  void CalibrateInitialPosition(void);

  /**
   * Change ride mode. Each call <mode = mode+1>
   * Resets mode automatically if overflowed.
   * @return current ride mode
   */
  rideMode ChangeRideMode(void);

  /**
   * Return position of hall sensor.
   * Position is remapped accordingly to <rideMode> value
   * @return 0 - neutral, negative values when braking, positive when
   * accelerating
   */
  int GetPosition(void);
};