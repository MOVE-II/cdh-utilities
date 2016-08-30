cmake_minimum_required(VERSION 3.2)

project(moveio-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

find_package(moveio REQUIRED)
include_directories(${MOVEIO_INCLUDE_DIRS})

add_executable(spitest spitest.cpp)
target_link_libraries(spitest ${MOVEIO_LIBRARY})

add_executable(pinmap pinmap.cpp)
target_link_libraries(pinmap ${MOVEIO_LIBRARY})

add_executable(gpio_test gpio_test.cpp)
target_link_libraries(gpio_test ${MOVEIO_LIBRARY})

add_executable(gpio_wait gpio_wait.cpp)
target_link_libraries(gpio_wait ${MOVEIO_LIBRARY})

add_executable(i2c_test i2c_test.cpp)
target_link_libraries(i2c_test ${MOVEIO_LIBRARY})

