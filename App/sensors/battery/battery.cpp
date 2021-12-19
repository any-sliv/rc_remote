/*
 * battery.cpp
 *
 *  Created on: Nov 26, 2021
 *      Author: macsli
 */

#include "battery.hpp"

float Battery::adcValueToVoltage(uint32_t val) {
  return ((float)val / 4095) * 3.3;
}

uint8_t Battery::GetPercent(void) {
  // times 2 due to we have voltage divier of 2
  float voltage = adcValueToVoltage(GetAdcValue(&channelConfig)) * 2;

  if(voltage > bC[12]) return 101;

  for (uint8_t i = 0; i < 11; i++) {
    if (voltage > bC[i]) {
      if (i == 10)
        return i * 10;
      else if (voltage < bC[i + 1]) {
        return i * 10;
      }
    } 
    else
      return i * 10;
  }
  return 0;
}

Battery::Battery(ADC_ChannelConfTypeDef *channel, GPIO_TypeDef *portAddr) {
  pinChrgEn = Gpio(portAddr, CHARGER_ENABLE_Pin, 1, OUTPUT);
  pinChrg = Gpio(portAddr, CHARGER_CHARGE_Pin, 0, INPUT, PULLUP);
  pinStdby = Gpio(portAddr, CHARGER_STANDBY_Pin, 0, INPUT, PULLUP);

  channelConfig = *channel;
}
Battery::Battery(void) {
  pinChrgEn = Gpio(CHARGER_CHARGE_GPIO_Port, CHARGER_ENABLE_Pin, 1, OUTPUT);
  pinChrg = Gpio(CHARGER_CHARGE_GPIO_Port, CHARGER_CHARGE_Pin, 0, INPUT, PULLUP);
  pinStdby = Gpio(CHARGER_CHARGE_GPIO_Port, CHARGER_STANDBY_Pin, 0, INPUT, PULLUP);

  // Enable charging
  pinChrgEn.Set();
 }

Battery::ChargeState Battery::GetChargeState(void) {
  // Values of readouts taken from charger datasheet
  if (pinChrg.Read() == false && pinStdby.Read() == true) return CHARGING;
  else if (pinChrg.Read() == true && pinStdby.Read() == false) return TERMINATED;
  else return NO_VOLTAGE;
}