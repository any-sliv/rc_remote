/*
 * ss49.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "ss49.hpp"

extern ADC_HandleTypeDef hadc;

SS49::SS49() {
    channelConfig.Channel = ADC_CHANNEL_0;
    channelConfig.Rank = 1;
    channelConfig.SamplingTime = conversionTimeout;
}

int SS49::GetPosition(void) {

}