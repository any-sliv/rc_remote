/*
 * battery.hpp
 *
 *  Created on: Oct 29, 2021
 *      Author: macsli
 */

#include "stm32l1xx_hal.h"
#include "adc.h"
#include "analogRead.hpp"

class Battery : AnalogRead {
private:
    const ADC_ChannelConfTypeDef channelConfig = {
        ADC_CHANNEL_1,          /* channel */
        (uint32_t) 2,           /* rank */
        ADC_SAMPLETIME_48CYCLES /* conversionTimeout */
    };

    // As a Battery Capacity
    const float bC [11] =   {3.5, 3.57, 3.64, 3.71, 3.78, 
                            3.85, 3.92, 3.99, 4.06, 4.13, 4.2};

    float adcValueToVoltage(uint32_t val) {
        return (float)((4095 * 3.3) / val);
    }

public:
    /**
     * @return Percentage value of battery. '0' or multiple of 10
     */
    uint8_t BatteryPercent(void) {
        float adcVal = adcValueToVoltage(GetAdcValue(&channelConfig));

        for(uint8_t i = 0; i < 11; i++) {
            if((adcVal > bc[i]) && (adcVal < bc[i + 1])) return i * 10;
        }
    }
};
