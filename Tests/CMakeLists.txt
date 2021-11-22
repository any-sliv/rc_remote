cmake_minimum_required(VERSION 3.1)

Project(Testing C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(WORKSPACE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

include_directories(
    ${WORKSPACE_PATH}/App/gpio
    #${WORKSPACE_PATH}/App/nrf24
    #${WORKSPACE_PATH}/App/nrf24/drivers
    #${WORKSPACE_PATH}/App/logger
    #${WORKSPACE_PATH}/App/sensors
    #${WORKSPACE_PATH}/App/sensors/hall_sensor
    #${WORKSPACE_PATH}/App/sensors/battery
    #${WORKSPACE_PATH}/App/ws2812

    Fixtures
    Modules
    Modules/gpio
    Modules/gpio/fake_includes
    googletest
    googletest/include/gtest
    ${GTEST_INCLUDE_DIRS}
    googletest/googletest/include
    googletest/googletest/include/gtest
)

file(GLOB_RECURSE SOURCES 
    main.cxx
    ${WORKSPACE_PATH}/App/gpio/*.*
    #${WORKSPACE_PATH}/App/nrf24/*.*
    #${WORKSPACE_PATH}/App/nrf24/drivers/*.*
    #${WORKSPACE_PATH}/App/logger/*.*
    #${WORKSPACE_PATH}/App/sensors/*.*
    #${WORKSPACE_PATH}/App/sensors/hall_sensor/*.*
    #${WORKSPACE_PATH}/App/sensors/battery/*.*
    #${WORKSPACE_PATH}/App/ws2812/*.*
    Fixtures/*.*
    Modules/*.*
    Modules/gpio/*.*
    Modules/gpio/fake_includes/*.*
)

add_subdirectory(googletest)
find_package(GTest REQUIRED)

add_executable(utest ${SOURCES})

target_link_libraries(
    utest
    gtest
    pthread
)