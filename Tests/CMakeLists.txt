cmake_minimum_required(VERSION 3.1)

Project(Testing C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(ONE_UP ../)

include_directories(
    ${ONE_UP}/App/gpio
    ${ONE_UP}/App/nrf24
    ${ONE_UP}/App/nrf24/drivers
    ${ONE_UP}/App/logger
    ${ONE_UP}/App/sensors
    ${ONE_UP}/App/sensors/hall_sensor
    ${ONE_UP}/App/sensors/battery
    ${ONE_UP}/App/ws2812

    Fixtures
    Modules
    Modules/gpio
    Modules/gpio/fake_includes
    googletest
    googletest/include/gtest
)

file(GLOB_RECURSE SOURCES 
    ${ONE_UP}/"App/gpio/*.*"
    ${ONE_UP}/"App/nrf24/*.*"
    ${ONE_UP}/"App/nrf24/drivers/*.*"
    ${ONE_UP}/"App/logger/*.*"
    ${ONE_UP}/"App/sensors/*.*"
    ${ONE_UP}/"App/sensors/hall_sensor/*.*"
    ${ONE_UP}/"App/sensors/battery/*.*"
    ${ONE_UP}/"App/ws2812/*.*"
    "main.cxx"
    "Fixtures/*.*"
    "Modules/*.*"
)

add_subdirectory(googletest)
find_package(GTest REQUIRED)
include_directories(
    ${GTEST_INCLUDE_DIRS}
    googletest/googletest/include
    googletest/googletest/include/gtest
)

add_executable(utest ${SOURCES})

target_link_libraries(
    utest
    gtest
    pthread
)