project(SPI_parallel_access_test_slave-mock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(SPI_pat_slave-mock ${SOURCE_FILES})