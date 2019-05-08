#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libCom )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libUtils )

# Add all *.c* files as source code
set(SrcFiles
  Keithley2000.cpp
  Fluke8842.cpp
  HP3478A.cpp
  PM6680)

add_library(METER SHARED ${SrcFiles})
add_dependencies(METER COM)
target_link_libraries(METER COM)
