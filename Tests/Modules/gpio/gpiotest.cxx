#include <gtest/gtest.h>
#include "stm32l1xx_hal_gpio.h"

TEST(gpioApp, init) {
    GTEST_ASSERT_EQ(getone(), 1);
}