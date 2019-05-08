#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libCom/ )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libUtils/ )

link_directories( ${CMAKE_BINARY_DIR}/lib )

# Add all *.c* files as source code
set(SrcFiles AgilentPs.cpp
  SorensenPs.cpp
  RigolDP832.cpp
  Keithley24XX.cpp
  TTIMX180TPPs.cpp
  TTITSX1820PPs.cpp)

add_library(PS SHARED ${SrcFiles})
add_dependencies(PS COM)
target_link_libraries(PS COM)
