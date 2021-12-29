#ifndef __STM32L1xx_HAL_ADC_H
#define __STM32L1xx_HAL_ADC_H

#include "stdint.h"
#include "stm32l1xx_hal_def.h"

#define HAL_ADC_STATE_RESET 0
#define ADC_SAMPLETIME_48CYCLES 0

typedef struct
{
  uint32_t ClockPrescaler;        /*!< Select ADC clock source (asynchronous clock derived from HSI RC oscillator) and clock prescaler.
                                       This parameter can be a value of @ref ADC_ClockPrescaler
                                       Note: In case of usage of channels on injected group, ADC frequency should be lower than AHB clock frequency /4 for resolution 12 or 10 bits, 
                                             AHB clock frequency /3 for resolution 8 bits, AHB clock frequency /2 for resolution 6 bits.
                                       Note: HSI RC oscillator must be preliminarily enabled at RCC top level. */
  uint32_t Resolution;            /*!< Configures the ADC resolution. 
                                       This parameter can be a value of @ref ADC_Resolution */
  uint32_t DataAlign;             /*!< Specifies ADC data alignment to right (MSB on register bit 11 and LSB on register bit 0) (default setting)
                                       or to left (if regular group: MSB on register bit 15 and LSB on register bit 4, if injected group (MSB kept as signed value due to potential negative value after offset application): MSB on register bit 14 and LSB on register bit 3).
                                       This parameter can be a value of @ref ADC_Data_align */
  uint32_t ScanConvMode;          /*!< Configures the sequencer of regular and injected groups.
                                       This parameter can be associated to parameter 'DiscontinuousConvMode' to have main sequence subdivided in successive parts.
                                       If disabled: Conversion is performed in single mode (one channel converted, the one defined in rank 1).
                                                    Parameters 'NbrOfConversion' and 'InjectedNbrOfConversion' are discarded (equivalent to set to 1).
                                       If enabled:  Conversions are performed in sequence mode (multiple ranks defined by 'NbrOfConversion'/'InjectedNbrOfConversion' and each channel rank).
                                                    Scan direction is upward: from rank1 to rank 'n'.
                                       This parameter can be a value of @ref ADC_Scan_mode */
  uint32_t EOCSelection;          /*!< Specifies what EOC (End Of Conversion) flag is used for conversion by polling and interruption: end of conversion of each rank or complete sequence.
                                       This parameter can be a value of @ref ADC_EOCSelection.
                                       Note: For injected group, end of conversion (flag&IT) is raised only at the end of the sequence.
                                             Therefore, if end of conversion is set to end of each conversion, injected group should not be used with interruption (HAL_ADCEx_InjectedStart_IT)
                                             or polling (HAL_ADCEx_InjectedStart and HAL_ADCEx_InjectedPollForConversion). By the way, polling is still possible since driver will use an estimated timing for end of injected conversion.
                                       Note: If overrun feature is intended to be used, use ADC in mode 'interruption' (function HAL_ADC_Start_IT() ) with parameter EOCSelection set to end of each conversion or in mode 'transfer by DMA' (function HAL_ADC_Start_DMA()).
                                             If overrun feature is intended to be bypassed, use ADC in mode 'polling' or 'interruption' with parameter EOCSelection must be set to end of sequence */
  uint32_t LowPowerAutoWait;      /*!< Selects the dynamic low power Auto Delay: new conversion start only when the previous
                                       conversion (for regular group) or previous sequence (for injected group) has been treated by user software, using function HAL_ADC_GetValue() or HAL_ADCEx_InjectedGetValue().
                                       This feature automatically adapts the speed of ADC to the speed of the system that reads the data. Moreover, this avoids risk of overrun for low frequency applications. 
                                       This parameter can be a value of @ref ADC_LowPowerAutoWait.
                                       Note: Do not use with interruption or DMA (HAL_ADC_Start_IT(), HAL_ADC_Start_DMA()) since they have to clear immediately the EOC flag to free the IRQ vector sequencer.
                                             Do use with polling: 1. Start conversion with HAL_ADC_Start(), 2. Later on, when conversion data is needed: use HAL_ADC_PollForConversion() to ensure that conversion is completed
                                             and use HAL_ADC_GetValue() to retrieve conversion result and trig another conversion (in case of usage of injected group, use the equivalent functions HAL_ADCExInjected_Start(), HAL_ADCEx_InjectedGetValue(), ...).
                                       Note: ADC clock latency and some timing constraints depending on clock prescaler have to be taken into account: refer to reference manual (register ADC_CR2 bit DELS description). */
  uint32_t LowPowerAutoPowerOff;  /*!< Selects the auto-off mode: the ADC automatically powers-off after a conversion and automatically wakes-up when a new conversion is triggered (with startup time between trigger and start of sampling).
                                       This feature can be combined with automatic wait mode (parameter 'LowPowerAutoWait').
                                       This parameter can be a value of @ref ADC_LowPowerAutoPowerOff. */
  uint32_t ChannelsBank;          /*!< Selects the ADC channels bank.
                                       This parameter can be a value of @ref ADC_ChannelsBank.
                                       Note: Banks availability depends on devices categories.
                                       Note: To change bank selection on the fly, without going through execution of 'HAL_ADC_Init()', macro '__HAL_ADC_CHANNELS_BANK()' can be used directly. */
  
#if defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
  uint32_t NbrOfConversion;       /*!< Specifies the number of ranks that will be converted within the regular group sequencer.
                                       To use regular group sequencer and convert several ranks, parameter 'ScanConvMode' must be enabled.
                                       This parameter must be a number between Min_Data = 1 and Max_Data = 28. */
#else
  uint32_t NbrOfConversion;       /*!< Specifies the number of ranks that will be converted within the regular group sequencer.
                                       To use regular group sequencer and convert several ranks, parameter 'ScanConvMode' must be enabled.
                                       This parameter must be a number between Min_Data = 1 and Max_Data = 27. */
#endif /* STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
  uint32_t NbrOfDiscConversion;   /*!< Specifies the number of discontinuous conversions in which the  main sequence of regular group (parameter NbrOfConversion) will be subdivided.
                                       If parameter 'DiscontinuousConvMode' is disabled, this parameter is discarded.
                                       This parameter must be a number between Min_Data = 1 and Max_Data = 8. */
  uint32_t ExternalTrigConv;      /*!< Selects the external event used to trigger the conversion start of regular group.
                                       If set to ADC_SOFTWARE_START, external triggers are disabled.
                                       If set to external trigger source, triggering is on event rising edge by default.
                                       This parameter can be a value of @ref ADC_External_trigger_source_Regular */
  uint32_t ExternalTrigConvEdge;  /*!< Selects the external trigger edge of regular group.
                                       If trigger is set to ADC_SOFTWARE_START, this parameter is discarded.
                                       This parameter can be a value of @ref ADC_External_trigger_edge_Regular */
}ADC_InitTypeDef;

/** 
  * @brief  Structure definition of ADC channel for regular group   
  * @note   The setting of these parameters with function HAL_ADC_ConfigChannel() is conditioned to ADC state.
  *         ADC can be either disabled or enabled without conversion on going on regular group.
  */ 
typedef struct 
{
  uint32_t Channel;                /*!< Specifies the channel to configure into ADC regular group.
                                        This parameter can be a value of @ref ADC_channels
                                        Note: Depending on devices, some channels may not be available on package pins. Refer to device datasheet for channels availability.
                                              Maximum number of channels by device category (without taking in account each device package constraints): 
                                              STM32L1 category 1, 2: 24 channels on external pins + 3 channels on internal measurement paths (VrefInt, Temp sensor, Vcomp): Channel 0 to channel 26.
                                              STM32L1 category 3:    25 channels on external pins + 3 channels on internal measurement paths (VrefInt, Temp sensor, Vcomp): Channel 0 to channel 26, 1 additional channel in bank B. Note: OPAMP1 and OPAMP2 are connected internally but not increasing internal channels number: they are sharing ADC input with external channels ADC_IN3 and ADC_IN8.
                                              STM32L1 category 4, 5: 40 channels on external pins + 3 channels on internal measurement paths (VrefInt, Temp sensor, Vcomp): Channel 0 to channel 31, 11 additional channels in bank B. Note: OPAMP1 and OPAMP2 are connected internally but not increasing internal channels number: they are sharing ADC input with external channels ADC_IN3 and ADC_IN8.
                                        Note: In case of peripherals OPAMPx not used: 3 channels (3, 8, 13) can be configured as direct channels (fast channels). Refer to macro ' __HAL_ADC_CHANNEL_SPEED_FAST() '.
                                        Note: In case of peripheral OPAMP3 and ADC channel OPAMP3 used (OPAMP3 available on STM32L1 devices Cat.4 only): the analog switch COMP1_SW1 must be closed. Refer to macro: ' __HAL_OPAMP_OPAMP3OUT_CONNECT_ADC_COMP1() '. */
  uint32_t Rank;                   /*!< Specifies the rank in the regular group sequencer.
                                        This parameter can be a value of @ref ADC_regular_rank
                                        Note: In case of need to disable a channel or change order of conversion sequencer, rank containing a previous channel setting can be overwritten by the new channel setting (or parameter number of conversions can be adjusted) */
  uint32_t SamplingTime;           /*!< Sampling time value to be set for the selected channel.
                                        Unit: ADC clock cycles
                                        Conversion time is the addition of sampling time and processing time (12 ADC clock cycles at ADC resolution 12 bits, 11 cycles at 10 bits, 9 cycles at 8 bits, 7 cycles at 6 bits).
                                        This parameter can be a value of @ref ADC_sampling_times
                                        Caution: This parameter updates the parameter property of the channel, that can be used into regular and/or injected groups.
                                                 If this same channel has been previously configured in the other group (regular/injected), it will be updated to last setting.
                                        Note: In case of usage of internal measurement channels (VrefInt/Vbat/TempSensor),
                                              sampling time constraints must be respected (sampling time can be adjusted in function of ADC clock frequency and sampling time setting)
                                              Refer to device datasheet for timings values, parameters TS_vrefint, TS_temp (values rough order: 4us min). */
}ADC_ChannelConfTypeDef;

typedef struct
{
      uint32_t SR;           /*!< ADC status register,                         Address offset: 0x00 */
      uint32_t CR1;          /*!< ADC control register 1,                      Address offset: 0x04 */
      uint32_t CR2;          /*!< ADC control register 2,                      Address offset: 0x08 */
      uint32_t SMPR1;        /*!< ADC sample time register 1,                  Address offset: 0x0C */
      uint32_t SMPR2;        /*!< ADC sample time register 2,                  Address offset: 0x10 */
      uint32_t SMPR3;        /*!< ADC sample time register 3,                  Address offset: 0x14 */
      uint32_t JOFR1;        /*!< ADC injected channel data offset register 1, Address offset: 0x18 */
      uint32_t JOFR2;        /*!< ADC injected channel data offset register 2, Address offset: 0x1C */
      uint32_t JOFR3;        /*!< ADC injected channel data offset register 3, Address offset: 0x20 */
      uint32_t JOFR4;        /*!< ADC injected channel data offset register 4, Address offset: 0x24 */
      uint32_t HTR;          /*!< ADC watchdog higher threshold register,      Address offset: 0x28 */
      uint32_t LTR;          /*!< ADC watchdog lower threshold register,       Address offset: 0x2C */
      uint32_t SQR1;         /*!< ADC regular sequence register 1,             Address offset: 0x30 */
      uint32_t SQR2;         /*!< ADC regular sequence register 2,             Address offset: 0x34 */
      uint32_t SQR3;         /*!< ADC regular sequence register 3,             Address offset: 0x38 */
      uint32_t SQR4;         /*!< ADC regular sequence register 4,             Address offset: 0x3C */
      uint32_t SQR5;         /*!< ADC regular sequence register 5,             Address offset: 0x40 */
      uint32_t JSQR;         /*!< ADC injected sequence register,              Address offset: 0x44 */
      uint32_t JDR1;         /*!< ADC injected data register 1,                Address offset: 0x48 */
      uint32_t JDR2;         /*!< ADC injected data register 2,                Address offset: 0x4C */
      uint32_t JDR3;         /*!< ADC injected data register 3,                Address offset: 0x50 */
      uint32_t JDR4;         /*!< ADC injected data register 4,                Address offset: 0x54 */
      uint32_t DR;           /*!< ADC regular data register,                   Address offset: 0x58 */
  uint32_t RESERVED;          /*!< Reserved,                                    Address offset: 0x5C */
} ADC_TypeDef;

typedef struct
{
  ADC_TypeDef                   *Instance;              /*!< Register base address */

  ADC_InitTypeDef               Init;                   /*!< ADC required parameters */

  uint32_t                 NbrOfConversionRank ;   /*!< ADC conversion rank counter */

  uint32_t                 State;                  /*!< ADC communication state (bitmap of ADC states) */

  uint32_t                 ErrorCode;              /*!< ADC Error code */

}ADC_HandleTypeDef;

void HAL_Delay(int t);

void setConversionFlag(bool val);

void setAdcReturnValue(uint32_t val);

void MX_ADC_Init(void);

void HAL_ADC_Start(ADC_HandleTypeDef * adc);

void HAL_ADC_Stop(ADC_HandleTypeDef * adc);

void HAL_ADC_ConfigChannel(ADC_HandleTypeDef * adc, ADC_ChannelConfTypeDef * channel);

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef * adc, uint32_t timeout);

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef * adc);

#endif