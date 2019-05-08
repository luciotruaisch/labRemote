#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libCom/ )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libUtils/ )

# Add all *.c* files as source code
set(SrcFiles PG8133A.cpp
  WF33120A.cpp)

add_library(WAVEFORMGEN SHARED ${SrcFiles})
add_dependencies(WAVEFORMGEN COM)
target_link_libraries(WAVEFORMGEN COM)
