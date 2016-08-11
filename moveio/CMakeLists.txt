cmake_minimum_required(VERSION 3.2)

project(moveio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_library(moveio src/GPIOPin.cpp src/SPIDevice.cpp src/PinMapper.cpp src/I2CDevice.cpp)
target_include_directories (moveio PUBLIC include/)

# install targets
install(TARGETS moveio ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/moveio)
