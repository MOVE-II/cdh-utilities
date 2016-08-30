cmake_minimum_required(VERSION 3.2)

project(moveio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(MOVEIO_SOURCES
	src/GPIOPin.cpp
	src/SPIDevice.cpp
	src/PinMapper.cpp
	src/I2CDevice.cpp
)

add_library(moveio SHARED ${MOVEIO_SOURCES})
target_include_directories (moveio PUBLIC include/)

# install targets
set(MOVEIO_NAME moveio)

install(TARGETS moveio LIBRARY DESTINATION lib/${MOVEIO_NAME})
install(DIRECTORY include/ DESTINATION include/${MOVEIO_NAME}/moveio)
install(FILES moveio-config.cmake DESTINATION lib/${MOVEIO_NAME})

