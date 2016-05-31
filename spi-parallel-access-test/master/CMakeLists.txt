project(SPI_parallel_access_test_master)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp DataSender.cpp)
add_executable(SPI_pat_master ${SOURCE_FILES})

install(TARGETS SPI_pat_master
        RUNTIME DESTINATION bin)