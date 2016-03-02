project(of)

cmake_minimum_required(VERSION 2.8)

find_package(GDAL REQUIRED)

include_directories(${GDAL_INCLUDE_DIR})

set(SRC_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

file(GLOB OF_SRC_FILES ${SRC_ABSOLUTE_ROOT_DIR}/*.cpp)
file(GLOB OF_HDR_FILES ${SRC_ABSOLUTE_ROOT_DIR}/*.h)

add_executable(${PROJECT_NAME} ${OF_SRC_FILES} ${OF_HDR_FILES} )

target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
