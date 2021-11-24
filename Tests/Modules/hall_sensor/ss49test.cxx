#include <gtest/gtest.h>
#include "ss49.hpp"

ADC_HandleTypeDef hadc;

class hallSensorTest : public ::testing::Test {
public:
    ADC_ChannelConfTypeDef channelConfig = {
        0,          /* channel */
        (uint32_t) 1,           /* rank */
        0 /* conversionTimeout */
    };

    // Only matters 0:4095 (max res of 12bit AD converter)
    uint32_t setVal = (std::rand() % 3500) + 100;
    uint32_t period = 200;
    SS49 * sensor;
    
    void SetUp() override {
        setConversionFlag(true);
        sensor = new SS49(&channelConfig);
    }

    void TearDown() override {
        delete sensor;
    }
};

TEST_F(hallSensorTest, GetPosition) {
    EXPECT_TRUE(sensor);

    setConversionFlag(true);
    setAdcReturnValue(setVal);

    EXPECT_EQ(sensor->GetPosition(), setVal);
}

TEST_F(hallSensorTest, CalibrationConstantValue) {
    setConversionFlag(true);
    setAdcReturnValue(setVal);

    sensor->CalibrateInitialPosition(period);
    // Should be 0 due to position returns (actual ADC value - neutralPos) 
    // And actual ADC val is neutralPos
    EXPECT_EQ(sensor->GetPosition(), 0) << "setVal was: " << setVal;
}

TEST_F(hallSensorTest, AfterCalibrationPositiveValue) {
    uint32_t addVal = 20;
    setConversionFlag(true);
    setAdcReturnValue(setVal);

    sensor->CalibrateInitialPosition(period);
    // Should be 0 due to position returns (actual ADC value - neutralPos) 
    // And actual ADC val is neutralPos
    setAdcReturnValue(setVal + addVal);
    EXPECT_EQ(sensor->GetPosition(), addVal) << "setVal was: " << setVal;
}

TEST_F(hallSensorTest, AfterCalibrationNegativeValue) {
    uint32_t subtractVal = 20;
    setConversionFlag(true);
    setAdcReturnValue(setVal);

    sensor->CalibrateInitialPosition(period);
    // Should be 0 due to position returns (actual ADC value - neutralPos) 
    // And actual ADC val is neutralPos
    setAdcReturnValue(setVal - subtractVal);
    EXPECT_EQ(sensor->GetPosition(), (0-subtractVal)) << "setVal was: " << setVal;
}