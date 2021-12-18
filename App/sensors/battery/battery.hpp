/*
 * battery.hpp
 *
 *  Created on: Oct 29, 2021
 *      Author: macsli
 */

extern "C" {
#include "adc.h"
#include "gpio.h"
#include "stm32l1xx_hal.h"
}  // extern C close
#include "analogRead.hpp"
#include "gpioApp.hpp"

class Battery : AnalogRead {
 private:
  Gpio pinChrgEn;    // charge enable
  Gpio pinChrg;    // read charge state
  Gpio pinStdby;   // read charge state
  ADC_ChannelConfTypeDef channelConfig = {
    .Channel = 1,
    .Rank = 1,
    .SamplingTime = ADC_SAMPLETIME_48CYCLES
  };

  // As a Battery Capacity
  const float bC[11] = {3.6,  3.65, 3.69, 3.74, 3.8, 3.84,
                        3.87, 3.95, 4.02, 4.10, 4.2};

 public:
  /**
   * @param channel pointer to ADC channel configuration structure
   * @param gpioAddr has default value, use only for testing!
   * Warning: all pins within are initialized with one default PORTx address
   * If they differ, introduce new default arguments
   */
  Battery(ADC_ChannelConfTypeDef *channel,
          GPIO_TypeDef *portAddr = CHARGER_ENABLE_GPIO_Port);

  Battery(void);  

  float adcValueToVoltage(uint32_t val);

  enum ChargeState { 
    TERMINATED,
    CHARGING,
    NO_BATTERY,
  };

  /**
   * @return Percentage value of battery. '0' or multiple of 10
   */
  uint8_t GetPercent(void);

  ChargeState GetChargeState(void);
};
