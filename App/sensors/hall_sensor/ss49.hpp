/*
 * ss49.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: macsli
 */

#include "stm32l1xx_hal.h"
#include "analogRead.hpp"

class SS49 : AnalogRead{
private:

public:
    SS49();

    /**
     * Return position of hall sensor.
     * @return 0 - neutral, negative values when braking, positive when accelerating
     */
    int GetPosition(void);
};