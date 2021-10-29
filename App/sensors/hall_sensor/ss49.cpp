/*
 * ss49.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include <vector>
#include "ss49.hpp"

extern ADC_HandleTypeDef hadc;

SS49::SS49() {
    neutralThrottle = (int)CalibrateInitialPosition();
}

int SS49::GetPosition(void) {
    return (GetAdcValue(&channelConfig) - neutralThrottle);
}

uint32_t SS49::CalibrateInitialPosition(void) {
    std::vector<uint32_t> samplesVector;
    
    for(int i = 0; i < calibrationSamplingTime; i++) {
        samplesVector.push_back(
                    GetAdcValue(&channelConfig));
        HAL_Delay(1);
    }

    uint32_t sum;
    for (auto && sample : samplesVector) {
        sum += sample;
    }
    
    return (sum / (uint32_t)samplesVector.size());
}

//todo add save in flash previous position and check if current calibration
//todo is not much higher than previous position
//todo but until you write to flash read and decide on write
//todo to prevent often write cycles