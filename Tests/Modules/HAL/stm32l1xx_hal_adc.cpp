extern "C" {
#include "stm32l1xx_hal_adc.h"
} // extern C close
#include <chrono>
#include <thread>

ADC_HandleTypeDef hadc;

uint32_t adcReturnValue = 0;
bool conversionFlag = false;

void setConversionFlag(bool val) {
    conversionFlag = val;
}

void setAdcReturnValue(uint32_t val) {
    adcReturnValue = val;
}

void HAL_Delay(int t) {};

void MX_ADC_Init(void) {};

void HAL_ADC_Start(ADC_HandleTypeDef * adc) {};

void HAL_ADC_Stop(ADC_HandleTypeDef * adc) {};

void HAL_ADC_ConfigChannel(ADC_HandleTypeDef * adc, ADC_ChannelConfTypeDef * channel) {};

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef * adc, uint32_t timeout) {
    return (conversionFlag ? HAL_OK : HAL_BUSY);
}

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef * adc) {
    if(conversionFlag) return adcReturnValue;
    else return 0xFFFFFFFF;
}