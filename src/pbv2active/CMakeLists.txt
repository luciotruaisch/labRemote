# add global dependencies
include_directories( ../libDevCom )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles PBv2TB.cpp
)

add_library(pbv2active SHARED ${SrcFiles})
target_link_libraries(pbv2active -lDevCom)

add_subdirectory(tools)
