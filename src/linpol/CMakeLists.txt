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

  
 LinPOL12VTestTools.cpp
 )

if ( ${LIBFTDI_FOUND} )
  set(SrcFiles ${SrcFiles}
    EndeavourRawFTDI.cpp)
endif()

add_library(linpol SHARED ${SrcFiles})
target_link_libraries(linpol -lGPIB -lDevCom ${LIBFTDI_LIBRARIES})
add_dependencies(linpol DevCom GPIB)

add_subdirectory(tools)
