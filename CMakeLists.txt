cmake_minimum_required(VERSION 2.6)
project(catlogger)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -pthread")

# Set variables
include(${CMAKE_SOURCE_DIR}/cmake/SetEnv.cmake)

# Library sources
add_subdirectory(${LIBRARY_FOLDER})

# Library examples
add_subdirectory(test)

# Install targets
include(${CMAKE_SOURCE_DIR}/cmake/InstallConfig.cmake)
