# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# where is the target environment 

SET(CMAKE_FIND_ROOT_PATH /opt/move-binaries/)
SET(CMAKE_STAGING_PREFIX /opt/move-binaries/usr)

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# prevent removing of RPATH variable from installed executables
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
