#include <gtest/gtest.h>
// Mocked HAL module
//#include "stm32l1xx_hal_gpio.h"
#include "gpioApp.hpp"

class gpioAppTest : public ::testing::Test {
public:
    GPIO_TypeDef * portAddr = NULL;

    void SetUp() override {
        portAddr = new GPIO_TypeDef;
    }

    void TearDown() override {
        delete portAddr;
    }
};

TEST_F(gpioAppTest, setValue) {
    uint16_t pinNumber = 4;
    Gpio * pin = new Gpio{portAddr, pinNumber};
    EXPECT_EQ(pin->GetPinNumber(), pinNumber);

    pin->Set();
    EXPECT_EQ(portAddr->ODR, (1 << pinNumber)); 
}

TEST_F(gpioAppTest, resetValue) {
    uint16_t pinNumber = 11;
    Gpio * pin = new Gpio{portAddr, pinNumber};
    pin->Set(1);

    EXPECT_EQ(portAddr->ODR, (1 << pinNumber)); 
    pin->Reset();
    EXPECT_EQ(portAddr->ODR, (0 << pinNumber)) << "Value not reset"; 
}

TEST_F(gpioAppTest, pinDuplicate) {
    uint16_t pinNumber = 7;
    Gpio * pin = new Gpio{portAddr, pinNumber};
    pin->Set();

    EXPECT_EQ(portAddr->ODR, (1 << pinNumber)); 
}
