/*
 * analogRead.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "adc.h"
#include "stm32l1xx_hal_adc.h"

extern ADC_HandleTypeDef hadc;

class AnalogRead {
private:
    const uint32_t conversionTimeout = 1; //[ms]

public:
    AnalogRead() {
        // If ADC not initialized yet - init!
        if(hadc.State == HAL_ADC_STATE_RESET) MX_ADC_Init();
    }

    /**
     * Blocking function
     * @param channel configuration of desired channel
     * @return value of ADC measurement
     */
    uint32_t GetAdcValue(const ADC_ChannelConfTypeDef * channel) {
        HAL_ADC_Start(&hadc);
        HAL_ADC_ConfigChannel(&hadc, (ADC_ChannelConfTypeDef *)channel);
        while(HAL_ADC_PollForConversion(&hadc, conversionTimeout) == HAL_BUSY);

        return HAL_ADC_GetValue(&hadc);
    }
};