/*
 * analogRead.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "adc.h"

extern ADC_HandleTypeDef hadc;

class AnalogRead {
private:
    bool isInitialized = false;

protected:
    const uint32_t conversionTimeout = 1; //[ms]
    ADC_ChannelConfTypeDef channelConfig = {0xFFFF};

public:
    AnalogRead() {
        if(!(isInitialized)) MX_ADC_Init();
    }

    ~AnalogRead() {

    }
    uint32_t GetAdcValue(void) {
        HAL_ADC_Start(&hadc);
        if(channelConfig.Channel == 0xFFFF) return -1;
        HAL_ADC_ConfigChannel(&hadc, &channelConfig);
        while(HAL_ADC_PollForConversion(&hadc, conversionTimeout) == HAL_BUSY);

        return HAL_ADC_GetValue(&hadc);
    }
};