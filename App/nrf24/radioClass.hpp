/*
 * radioClass.hpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#ifndef INC_RADIOCLASS_H_
#define INC_RADIOCLASS_H_

#include "spi.h"

class NRF24
{
private:
    // Safety flag for changing power mode
    bool isSleeping;

public:
    NRF24();

    /**
     * Transmit given payload, constant length
     * @param payload address 
     */
    bool Write(void * payload);

    /**
     * Check if any payload has been recieved by radio
     */
    bool IsAvailable(void);

    /**
     * Read recieved payload
     * @param data address
     */ 
    void Read(void * data);

    /**
     * Put radio into sleep mode
     */
    void Sleep(void);

    /**
     * Power up the radio. Contains blocking delay of 1.5 ms
     */
    void Wakeup(void);
};

#endif /* INC_RADIOCLASS_H_ */