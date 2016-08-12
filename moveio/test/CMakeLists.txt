cmake_minimum_required(VERSION 3.2)

project(moveio-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_executable(spitest spitest.cpp)
target_link_libraries(spitest moveio)

add_executable(pinmap pinmap.cpp)
target_link_libraries(pinmap moveio)

add_executable(gpio_test gpio_test.cpp)
target_link_libraries(gpio_test moveio)

add_executable(gpio_wait gpio_wait.cpp)
target_link_libraries(gpio_wait moveio)

add_executable(i2c_test i2c_test.cpp)
target_link_libraries(i2c_test moveio)
