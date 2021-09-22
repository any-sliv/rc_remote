/*
 * radioTask.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */
#ifdef __cplusplus 
extern "C" { 
#endif 

#include "radioTask.hpp"
#include "radioClass.hpp"
#include "cmsis_os.h"
#include "task.h"


#define RADIO_TASK_TIME_INTERVAL 5000

void radioTask(void const * argument)
{
    uint32_t taskTimer = (uint32_t) osKernelGetTickCount + RADIO_TASK_TIME_INTERVAL;
    NRF24 radio;
    void * rxData;

    for(;;) {
        if(radio.IsAvailable()) {
            radio.Read(rxData);
        }
        radio.Write((void *)"x");

        vTaskDelay(taskTimer);
    }
}


// --------------------------------------------------------
#ifdef __cplusplus 
} 
#endif 
