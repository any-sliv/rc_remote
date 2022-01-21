cmake_minimum_required(VERSION 3.1)

project(rc_remote)

set(MCU_FAMILY STM32L1xx)
set(MCU_MODEL STM32L151xBA)
set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -ffunction-sections
    -fdata-sections
    -mfloat-abi=soft
  )

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/startup_stm32l151xba.s)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32L151CBTxA_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CUBEMX_INC
${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
${CMAKE_CURRENT_SOURCE_DIR}/Drivers
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32L1xx_HAL_Driver/Inc 
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32L1xx_HAL_Driver/Inc/Legacy 
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/STM32L1xx_HAL_Driver/Inc
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32L1xx_HAL_Driver/Inc/Legacy
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/Device/ST/STM32L1xx_HAL_Driver/Include
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/Device/ST/STM32L1xx_HAL_Driver/Include/Legacy
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32L1xx/Include 
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include 
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
) # ------------------------------------------------
SET(PROJECT_INC
${CMAKE_CURRENT_SOURCE_DIR}/App/button
${CMAKE_CURRENT_SOURCE_DIR}/App/gpio
${CMAKE_CURRENT_SOURCE_DIR}/App/nrf24
${CMAKE_CURRENT_SOURCE_DIR}/App/nrf24/drivers
${CMAKE_CURRENT_SOURCE_DIR}/App/logger
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors/hall_sensor
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors/battery
${CMAKE_CURRENT_SOURCE_DIR}/App/ws2812
${CMAKE_CURRENT_SOURCE_DIR}/App/flash
${CMAKE_CURRENT_SOURCE_DIR}/App/power
)
# /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-

file(GLOB_RECURSE CUBEMX_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/MemMang/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.*
${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32L1xx_HAL_Driver/*.*
) # ------------------------------------------------
file(GLOB_RECURSE PROJECT_SRC FOLLOW_SYMLINKS
${CMAKE_CURRENT_SOURCE_DIR}/App/button/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/gpio/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/nrf24/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/nrf24/drivers/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/logger/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors/hall_sensor/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/sensors/battery/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/ws2812/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/flash/*.*
${CMAKE_CURRENT_SOURCE_DIR}/App/power/*.*
) # ------------------------------------------------

add_executable(${EXECUTABLE}
  ${CUBEMX_SRC}
  ${PROJECT_SRC} 
  ${STARTUP_SCRIPT}
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  ${MCU_MODEL}
  USE_HAL_DRIVER
)

target_include_directories(${EXECUTABLE} PRIVATE
  ${CUBEMX_INC}
  ${PROJECT_INC}
)

target_compile_options(${EXECUTABLE} PRIVATE
  ${CPU_PARAMETERS}
  -Wno-unused-parameter
  -Wall
  -Wextra
  #-Wuseless-cast
  #-Wsuggest-override
  $<$<COMPILE_LANGUAGE:CXX>:
    -Wno-volatile
    -fno-rtti
    -fno-exceptions>
    #-Wold-style-cast
  $<$<CONFIG:Debug>:-O0 -g3 -ggdb>
  $<$<CONFIG:Release>:-Og -g0>
)

target_link_options( ${EXECUTABLE} PRIVATE
  -T${LINKER_SCRIPT}
  ${CPU_PARAMETERS}
  -Wl,-Map=${CMAKE_SOURCE_DIR}/Drivers/${CMAKE_PROJECT_NAME}.map
  -specs=nosys.specs
  #-specs=nano.specs
  -Wl,--start-group
  -lc
  -lm
  -lstdc++
  -lsupc++
  -Wl,--end-group
  -Wl,--gc-sections
  -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

set(HEX_FILE ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${PROJECT_NAME}.elf ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${PROJECT_NAME}.elf ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} Building ${BIN_FILE}"
)
