project(template)

# Flags for the compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# All cpp-files should get listed here
add_executable(myprogram
	src/hello.cpp
)

# Include needed libraries here, one target_link_libraries per library
# target_link_libraries(myprogram sampleLibrary)
target_link_libraries(myprogram moveio)

# All h-files go into the include folder, only need the folder name here
target_include_directories(myprogram PUBLIC include/)

# Installs programs to: /usr/local/bin
install(TARGETS myprogram
        RUNTIME DESTINATION bin)
