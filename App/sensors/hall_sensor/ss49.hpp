/*
 * ss49.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "stm32l1xx_hal.h"
#include "analogRead.hpp"

class SS49 : AnalogRead {
private:
    const ADC_ChannelConfTypeDef channelConfig = {
        ADC_CHANNEL_0,          /* channel */
        (uint32_t) 1,           /* rank */
        ADC_SAMPLETIME_48CYCLES /* conversionTimeout */
    };

    // Each sample colected once a milisecond
    const int calibrationSamplingTime = 200;

    //todo try to make it constant and assign value after init
    int neutralThrottle;

    /**
     * Blocking function
     * @return Value of average throttle position measured
     * in time given by <this>.calibrationSamplingTime * 1
     */
    uint32_t CalibrateInitialPosition(void);

public:
    SS49();

    /**
     * Return position of hall sensor.
     * @return 0 - neutral, negative values when braking, positive when accelerating
     */
    int GetPosition(void);

};