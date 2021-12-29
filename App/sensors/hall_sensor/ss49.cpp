/*
 * ss49.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "ss49.hpp"
#include "logger.hpp"
#include <vector>
#include <algorithm>

extern ADC_HandleTypeDef hadc;

#define SAMPLING_PERIOD 200

// todo add save in flash previous position and check if current calibration
// todo is not much higher than previous position
// todo but until you write to flash read and decide on write
// todo to prevent often write cycles

SS49::SS49(ADC_ChannelConfTypeDef *channel) { channelConfig = *channel; }

SS49::SS49(void) {
  //Logger::Log("SS49 hall sensor Init.");
  //CalibrateInitialPosition(samplingPeriod);
  CalibrateInitialPosition();
}

void SS49::CalibrateInitialPosition(uint32_t samplingPeriod) {
  std::vector<uint32_t> sample;
  uint32_t sum = 0;

  // Discard x/8 worst samples
  uint8_t discardSamples = samplingPeriod >> 3;

  for (uint32_t i = 0; i < samplingPeriod; i++) {
    sample.push_back(GetAdcValue(&channelConfig));
  }
  // Removing lowest and highest samples
  // Half of discard samples, because it = min+max
  for (int i = 0; i < (discardSamples >> 1); i++) {
    sample.erase(std::min_element(sample.begin(), sample.end()));
    sample.erase(std::max_element(sample.begin(), sample.end()));
  }

  for (auto &n : sample) sum += n;

  neutralThrottle = (int)(sum / sample.size());
}

void SS49::CalibrateInitialPosition(void) {
  uint32_t sum = 0;

  // Get all samples
  for (size_t i = 0; i < SAMPLING_PERIOD; i++) {
    sum += GetAdcValue(&channelConfig);
    HAL_Delay(1);
  }

  neutralThrottle = (int)(sum/SAMPLING_PERIOD);
}

int SS49::GetPosition(void) {
  return (int)GetAdcValue(&channelConfig) - neutralThrottle;
}