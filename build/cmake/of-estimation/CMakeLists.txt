#  Description: Optical Flow Estimation Command Line Tool.
#  Author: Douglas Uba

find_package(GDAL REQUIRED)

set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../third-party)

include_directories(${GDAL_INCLUDE_DIR} ${THIRD_PARTY_INCLUDE_DIR})

file(GLOB OF_ESTIMATION_FILE ${OF_ABSOLUTE_ROOT_DIR}/src/tools/of-estimation.cpp)

add_executable(of-estimation ${OF_ESTIMATION_FILE})

target_link_libraries(of-estimation of ${GDAL_LIBRARY})
