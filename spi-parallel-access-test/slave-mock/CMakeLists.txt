project(SPI_parallel_access_test_slave-mock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp GpioReader.cpp StateChangeCounter.cpp)
add_executable(SPI_pat_slave-mock ${SOURCE_FILES})

install(TARGETS SPI_pat_slave-mock
        RUNTIME DESTINATION bin)