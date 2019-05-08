#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
set(SrcFiles SerialCom.cpp)

add_library(COM SHARED ${SrcFiles})
