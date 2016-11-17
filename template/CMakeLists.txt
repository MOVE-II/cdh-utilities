cmake_minimum_required(VERSION 3.2)
project(template)

# find needed libraries
find_package(moveio REQUIRED)
include_directories(${MOVEIO_INCLUDE_DIRS})

# Flags for the compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

# Add one/multiple programs
add_executable(myprogram
	src/hello.cpp
)
target_include_directories(myprogram PUBLIC include/)
target_link_libraries(myprogram ${MOVEIO_LIBRARY})

install(TARGETS myprogram
        RUNTIME DESTINATION bin)
