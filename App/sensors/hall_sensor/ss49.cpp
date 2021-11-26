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
    std::vector <uint32_t> sample;
    uint32_t sum = 0;

    // Discard x/8 worst samples
    uint8_t discardSamples = samplingPeriod >> 3;

    for (int i = 0; i < samplingPeriod; i++) {
        sample.push_back(GetAdcValue(&channelConfig));
    }
    // Removing lowest and highest samples 
    // Half of discard samples, because it = min+max 
    for(int i = 0; i < (discardSamples >> 1); i++) {
        sample.erase(std::min_element(sample.begin(), sample.end()));
        sample.erase(std::max_element(sample.begin(), sample.end()));
    }

    for (auto &n : sample) sum += n;
    
    neutralThrottle = (int)(sum / sample.size());
}

int SS49::GetPosition(void)
{
    return (int)((int)GetAdcValue(&channelConfig) - neutralThrottle);
}
