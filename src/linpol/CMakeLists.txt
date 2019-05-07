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

if ( ${LIBFTDI_FOUND} )
  target_link_libraries(linpol ${LIBFTDI_LIBRARIES})
endif()
target_link_libraries(linpol -lGPIB -lDevCom)

add_dependencies(linpol DevCom GPIB)

add_subdirectory(tools)
