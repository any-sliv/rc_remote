/*
 * analogRead.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "adc.h"
#include "stm32l1xx_hal.h"

extern ADC_HandleTypeDef hadc;

class AnalogRead {
private:
    bool isInitialized = false;

public:
    AnalogRead() {
        if(!(isInitialized)) MX_ADC_Init();
    }

    ~AnalogRead() {
        HAL_ADC_DeInit(&hadc);
    }

    /**
     * @param channel configuration of desired channel
     * @return value of ADC measurement
     */
    uint32_t GetAdcValue(const ADC_ChannelConfTypeDef * channel) {
        HAL_ADC_Start(&hadc);
        if(channel->Channel == 0xFFFF) return -1;
        HAL_ADC_ConfigChannel(&hadc, (ADC_ChannelConfTypeDef *)channel);
        while(HAL_ADC_PollForConversion(&hadc, conversionTimeout) == HAL_BUSY);

        return HAL_ADC_GetValue(&hadc);
    }
};