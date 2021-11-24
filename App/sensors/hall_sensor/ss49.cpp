/*
 * ss49.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include <vector>
#include "ss49.hpp"

extern ADC_HandleTypeDef hadc;

// todo add save in flash previous position and check if current calibration
// todo is not much higher than previous position
// todo but until you write to flash read and decide on write
// todo to prevent often write cycles

SS49::SS49(ADC_ChannelConfTypeDef *channel)
{
    channelConfig = *channel;
}

void SS49::CalibrateInitialPosition(uint32_t samplingPeriod)
{
    uint32_t *sample = new uint32_t(samplingPeriod);
    uint32_t sum = 0;

    for (int i = 0; i < samplingPeriod; i++)
    {
        sample[i] = GetAdcValue(&channelConfig);
        HAL_Delay(1);
    }
    // additional processing here. all samples ready

    for (int i = 0; i < samplingPeriod; i++)
    {
        sum += sample[i];
    }
    neutralThrottle = (int)(sum / samplingPeriod);

    delete sample;
}

int SS49::GetPosition(void)
{
    return (int)((int)GetAdcValue(&channelConfig) - neutralThrottle);
}
