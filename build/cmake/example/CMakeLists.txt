#  Description: Example for Optical Flow.
#  Author: Douglas Uba

find_package(GDAL REQUIRED)

include_directories(${GDAL_INCLUDE_DIR} ${OF_ABSOLUTE_ROOT_DIR}/src)

file(GLOB EXAMPLE_FILE ${OF_ABSOLUTE_ROOT_DIR}/example/of-example.cpp)

add_executable(of-example ${EXAMPLE_FILE})

target_link_libraries(of-example of ${GDAL_LIBRARY})
