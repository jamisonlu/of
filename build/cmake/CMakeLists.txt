#  Description: Main CMake script for the Optical Flow build system.
#  Author: Douglas Uba

cmake_minimum_required(VERSION 2.8)

project(of)

set(OF_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

option(OF_BUILD_EXAMPLE "Build Optical Flow example?" ON)
option(OF_BUILD_ESTIMATION_TOOL "Build Optical Flow Estimation Tool?" ON)

add_subdirectory(of)

if(OF_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if(OF_BUILD_ESTIMATION_TOOL)
  add_subdirectory(of-estimation)
endif()
