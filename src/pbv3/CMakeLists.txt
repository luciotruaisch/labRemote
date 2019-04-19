if ( NOT ${LIBFTDI_FOUND} )
  message("Disabling pbv3 due to missing libftdi1")  
  return()
endif()


# add global dependencies
include_directories( ../libDevCom ../libGPIB/include)
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles
  ftdihelp.cpp

  EndeavourRaw.cpp
  EndeavourRawUIO.cpp
  EndeavourRawFTDI.cpp

  EndeavourCom.cpp
  EndeavourComException.cpp

  PBv3CommPatchSPICom.cpp

  AMACv2.cpp
  AMACv2Reg.cpp

  PBv3TestTools.cpp
  PBv3ConfigTools.cpp
  )

if ( ${LIBFTDI_FOUND} )
  set(SrcFiles ${SrcFiles}
    EndeavourRawFTDI.cpp)
endif()

add_library(pbv3 SHARED ${SrcFiles})
target_link_libraries(pbv3 -lGPIB -lDevCom ${LIBFTDI_LIBRARIES})
add_dependencies(pbv3 DevCom GPIB)

add_subdirectory(tools)
