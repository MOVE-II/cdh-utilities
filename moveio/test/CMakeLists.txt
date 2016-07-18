cmake_minimum_required(VERSION 3.2)

project(moveio-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_executable(spitest spitest.cpp)
target_link_libraries(spitest moveio)

add_executable(pinmap pinmap.cpp)
target_link_libraries(pinmap moveio)
