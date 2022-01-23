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
  const float internalVoltages[12] = {3.6,  3.65, 3.69, 3.74, 3.8, 3.84,
                        3.87, 3.95, 4.02, 4.10, 4.2, 4.3};

  // Ranges of capacity 
  const float externalValues[12] = {33.7, 34.8, 35.5, 35.9, 36.3, 36.7, 
                        37.3, 38.2, 39.2, 40.5, 42, 43};

  int externalBatteryValue;

 public:
  enum BatteryType {
    INTERNAL, //the one in remote
    EXTERNAL  //the one in skateboard
  };
  
  /**
   * @param channel pointer to ADC channel configuration structure
   * @param gpioAddr has default value, use only for testing!
   * Warning: all pins within are initialized with one default PORTx address
   * If they differ, introduce new default arguments
   */
  Battery(ADC_ChannelConfTypeDef *channel,
          GPIO_TypeDef *portAddr = CHARGER_ENABLE_GPIO_Port);

  Battery(void);  

  /**
   * @param val Please pass me value of external battery
   */
  void SetExternalBatteryValue(int val);

  float adcValueToVoltage(uint32_t val);

  enum ChargeState { 
    TERMINATED,
    CHARGING,
    NO_VOLTAGE,
  };

  /**
   * @return Percentage value of battery. '0' or multiple of 10
   */
  uint8_t GetPercent(BatteryType type);

  ChargeState GetChargeState(void);
};
