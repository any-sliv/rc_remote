cmake_minimum_required(VERSION 3.1)

project(rc_remote)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

set(EXECUTABLE ${CMAKE_PROJECT_NAME}.elf)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(SIZE arm-none-eabi-size)
enable_language(C CXX ASM)

set(CUBEMX_INC
Core/Inc
Drivers/STM32L1xx_HAL_Driver/Inc 
Drivers/STM32L1xx_HAL_Driver/Inc/Legacy 
Drivers/CMSIS/Include
Drivers/CMSIS/STM32L1xx_HAL_Driver/Inc
Drivers/STM32L1xx_HAL_Driver/Inc/Legacy
Drivers/Device/ST/STM32L1xx_HAL_Driver/Include
Drivers/Device/ST/STM32L1xx_HAL_Driver/Include/Legacy
Drivers/CMSIS/Device/ST/STM32L1xx/Include 
Drivers/CMSIS/Include
Middlewares/Third_Party/FreeRTOS/Source
Middlewares/Third_Party/FreeRTOS/Source/include 
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
) # ------------------------------------------------
SET(PROJECT_INC
App/gpio
App/nrf24
App/nrf24/drivers
App/logger
App/sensors
App/sensors/hall_sensor
App/sensors/battery
App/ws2812
)
# /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-

file(GLOB_RECURSE CUBEMX_SRC 
Core/Src/*.*
Middlewares/*.*
Middlewares/Third_Party/FreeRTOS/Source/*.*
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/*.*
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/*.*
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/MemMang/*.*
Drivers/*.*
Drivers/STM32L1xx_HAL_Driver/*.*
) # ------------------------------------------------
file(GLOB_RECURSE PROJECT_SRC
App/gpio/*.*
App/nrf24/*.*
App/nrf24/drivers/*.*
App/logger/*.*
App/sensors/*.*
App/sensors/hall_sensor/*.*
App/sensors/battery/*.*
App/ws2812/*.*
) # ------------------------------------------------

set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/Drivers/startup_stm32l151xba.s)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Drivers/STM32L151CBTxA_FLASH.ld)

set(CPU_PARAMETERS
  -mcpu=cortex-m4
  -mthumb
  -ffunction-sections
  -fdata-sections
)

add_executable( ${EXECUTABLE}
  ${CUBEMX_SRC}
  ${PROJECT_SRC} 
  ${LINKER_SCRIPT}
)

target_compile_definitions( ${EXECUTABLE} PRIVATE
  STM32L151xBA
  USE_HAL_DRIVER
)

target_include_directories( ${EXECUTABLE} PRIVATE
  ${PROJECT_INC}
  ${CUBEMX_INC}
)

target_compile_options( ${EXECUTABLE} PRIVATE
  ${CPU_PARAMETERS}
  -Wno-unused-parameter
  #-Wuseless-cast
  #-Wsuggest-override
  $<$<COMPILE_LANGUAGE:CXX>:
    -Wno-volatile>
    #-Wold-style-cast
  $<$<CONFIG:Debug>:-Og -g3 -ggdb>
  $<$<CONFIG:Release>:-Og -g0>
)

target_link_options( ${EXECUTABLE} PRIVATE
  -T${LINKER_SCRIPT}
  ${CPU_PARAMETERS}
  -Wl,-Map=${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.map
  --specs=nosys.specs
  -Wl,--start-group
  -lc
  -lm
  -lstdc++
  -lsupc++
  -Wl,--end-group
  -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

set(HEX_FILE ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${PROJECT_NAME} ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${PROJECT_NAME} ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} Building ${BIN_FILE}"
)
