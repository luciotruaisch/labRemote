# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
file(GLOB SrcFiles *.c*)
add_library(GPIB SHARED ${SrcFiles}) 
