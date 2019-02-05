# add global dependencies
include_directories( ../libDevCom ../libGPIB/include /usr/local/Cellar/nlohmann_json/3.1.2/include/nlohmann)
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
if ( ${LIBFTDI_FOUND} )
  target_link_libraries(pbv3 ${LIBFTDI_LIBRARIES})
endif()
target_link_libraries(pbv3 -lGPIB -lDevCom)
add_dependencies(pbv3 DevCom GPIB)

add_subdirectory(tools)
