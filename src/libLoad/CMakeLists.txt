#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
set(SrcFiles
  Bk85xx.cpp
  TTILD400P.cpp)

add_library(GPIB SHARED ${SrcFiles})
