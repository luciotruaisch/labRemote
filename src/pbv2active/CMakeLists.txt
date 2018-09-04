# add global dependencies
include_directories( ../libDevCom )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles PBv2TB.cpp
)

add_library(pbv2active SHARED ${SrcFiles}) 

add_subdirectory(tools)
