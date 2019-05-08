#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
set(SrcFiles
  Keithley2000.cpp
  Fluke8842.cpp
  HP3478A.cpp)

add_library(METER SHARED ${SrcFiles})
