/*
 * radioApp.hpp
 *
 *  Created on: Sep 15, 2021
 *      Author: macsli
 */

#ifndef INC_RADIOAPP_H_
#define INC_RADIOAPP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"
#include "nrf24.h"
#include "spi.h"
#include "task.h"

#ifdef __cplusplus
}
#endif

class NRF24 {
 private:
  // Safety flag for changing power mode
  bool isSleeping;

 public:
  NRF24();

  /**
   * Transmit given payload, constant length
   * @param payload address
   * @return success(1) / fail(0)
   */
  bool Write(void *payload);

  /**
   * Check if any payload has been recieved by radio
   */
  bool IsAvailable(void);

  /**
   * Read recieved payload
   * @param data address
   */
  void Read(void *data);

  /**
   * Put radio into sleep mode
   */
  void Sleep(void);

  /**
   * Power up the radio. Contains blocking delay of 1.5 ms
   */
  void Wakeup(void);
};

#endif /* INC_RADIOAPP_H_ */