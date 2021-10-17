#include "stm32l1xx_hal.h"
#include "stdint.h"
#include "gpioApp.hpp"
#include "gpioApp.hpp"
// gpio.h used only for pins naming - sourced from CubeMX
#include "gpio.h"

#define WS2812_LEDS_NUMBER 5

struct ws2812_diode_s {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

class Leds{
private:
    /**
     * DC/DC supplying leds pin state 0 - down, 1 - up
     */
    Gpio * ledsEnablePin = NULL;

    /**
     * Buffer storing data to be sent to LEDs from DMA.
     * Each field is one bit
     */
    uint16_t buffer[48];

    struct ws2812_diode_s wsLed [WS2812_LEDS_NUMBER];

    void loadBuffer(void);
public:
    Leds();

    /**
     * Put desired colour, in particular led into diodes structure.
     * Refresh required - do it manually!
     */
    void SetColour(ws2812_diode_s * wsStruct, uint8_t ledNumber);

    void Clear(void);

    void Refresh(void);
};