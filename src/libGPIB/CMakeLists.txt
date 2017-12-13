# Add the headers
include_directories( include )

# Add all *.c* files as source code
file(GLOB SrcFiles *.c*)
add_library(GPIB SHARED ${SrcFiles}) 
