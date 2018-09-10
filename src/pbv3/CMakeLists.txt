# add global dependencies
include_directories( ../libDevCom )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles EndeavourRaw.cpp
  EndeavourCom.cpp
  EndeavourComException.cpp
)

add_library(pbv3 SHARED ${SrcFiles}) 

add_subdirectory(tools)
