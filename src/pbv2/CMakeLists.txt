# add global dependencies
include_directories( ../libDevCom )
include_directories( ../libGPIB/include )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles PBv2TB.cpp
  PBv2Test.cpp
)

set(SrcFiles PBv2TB.cpp
  PBv2Test.cpp
)

add_library(pbv2 SHARED ${SrcFiles}) 
target_link_libraries(pbv2 -lDevCom -lGPIB)

add_subdirectory(tools)
add_subdirectory(singleboard)
