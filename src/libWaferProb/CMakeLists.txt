# Add the headers
include_directories( include )
include_directories( ../libGalil/include )
include_directories( ../libZaber/include )

# Add all *.c* files as source code
file(GLOB SrcFiles *.c*)
add_library(WaferProb SHARED ${SrcFiles}) 
target_link_libraries(WaferProb Galil Zaber)
#add_dependencies(WaferProb libGalil libZaber)
