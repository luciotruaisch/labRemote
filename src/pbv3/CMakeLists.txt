# add global dependencies
include_directories( ../libDevCom )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles
  EndeavourRaw.cpp
  EndeavourRawUIO.cpp
  EndeavourRawFTDI.cpp

  EndeavourCom.cpp
  EndeavourComException.cpp
  )

if ( ${LIBFTDI_FOUND} )
  set(SrcFiles ${SrcFiles}
    EndeavourRawFTDI.cpp)
endif()

add_library(pbv3 SHARED ${SrcFiles}) 
if ( ${LIBFTDI_FOUND} )
  target_link_libraries(pbv3 ${LIBFTDI_LIBRARIES})  
endif()

add_subdirectory(tools)
