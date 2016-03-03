#  Description: Core of Optical Flow implementation.
#  Author: Douglas Uba

if(WIN32)
  add_definitions(-DOFDLL)
endif()

file(GLOB OF_SRC_FILES ${OF_ABSOLUTE_ROOT_DIR}/src/of/*.cpp)
file(GLOB OF_HDR_FILES ${OF_ABSOLUTE_ROOT_DIR}/src/of/*.h)

set(OF_FILES ${OF_SRC_FILES} ${OF_HDR_FILES})

add_library(of SHARED ${OF_FILES})
