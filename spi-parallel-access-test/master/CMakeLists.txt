project(SPI_parallel_access_test_master)

find_package(Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp DataSender.cpp DataSenderThread.cpp)
add_executable(SPI_pat_master ${SOURCE_FILES})

target_link_libraries(SPI_pat_master ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS SPI_pat_master
        RUNTIME DESTINATION bin)