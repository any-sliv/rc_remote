#include <gtest/gtest.h>
#include "battery.hpp"


extern ADC_HandleTypeDef hadc;

class batteryTest : public ::testing::Test {
public:
    Battery * battery;
    GPIO_TypeDef * portAddr = NULL;

    ADC_ChannelConfTypeDef channelConfig = {
        0,          /* channel */
        (uint32_t) 2,           /* rank */
        0 /* conversionTimeout */
    };
    //                        3.5  3.65  3.84  4.10  4.25
    uint32_t adcValues[5] = {2172, 2265, 2385, 2549, 2637}; 
    uint32_t percentage[5] = {  0,   10,   50,   90, 100};
    uint32_t setVal = 0;

    void SetUp() override {
        // Clear hadc struct if previously initialized
        ADC_HandleTypeDef xhadc;
        hadc = xhadc;

        portAddr = new GPIO_TypeDef;
        battery = new Battery(&channelConfig, portAddr);

        setConversionFlag(true);
    }

    void TearDown() override {
        delete battery;
    }
};

TEST_F(batteryTest, Initial) {
    EXPECT_NE(battery, nullptr);
}

TEST_F(batteryTest, adcValueToVoltage)
{
    EXPECT_FLOAT_EQ(battery->adcValueToVoltage(4095), 3.3);
}

TEST_F(batteryTest, adcRead) {
    setAdcReturnValue(4095);
    EXPECT_EQ(HAL_ADC_GetValue(&hadc), 4095);
}

TEST_F(batteryTest, PercentageReadoutFromADC) {
    setConversionFlag(true);
    for (int i = 0; i < 5; i++) {
        setAdcReturnValue(adcValues[i]);
        EXPECT_EQ((uint32_t)battery->GetPercent(), percentage[i]) << "Failed at value: " << adcValues[i];
    }
}