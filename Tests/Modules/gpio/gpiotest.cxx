#include <gtest/gtest.h>
// Mocked HAL module
//#include "stm32l1xx_hal_gpio.h"
#include "gpioApp.hpp"

class gpioAppTest : public ::testing::Test {
public:
    GPIO_TypeDef * portAddr;

    void SetUp() override {
        portAddr = new GPIO_TypeDef;
    }

    void TearDown() override {
        delete portAddr;
    }
};

TEST_F(gpioAppTest, setValue) {
    Gpio * pin = new Gpio{portAddr, 0};
}