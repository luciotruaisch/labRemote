# add global dependencies
include_directories( ../libCom )
include_directories( ../libPs )
include_directories( ../libUtils )
include_directories( ../libMeter )
include_directories( ../libLoad )
include_directories( ../libDevCom )
include_directories( ../libGalil )
include_directories( ../libZaber )
include_directories( ../libWaferProb )
include_directories( ../libScope )

link_directories( ${CMAKE_BINARY_DIR}/lib )

if( ${ENABLE_FTDI} )
  add_definitions(-DFTDI)
endif()

if( ${ENABLE_SCOPE} )
  include_directories( ../libScope/include )
  include_directories( ${LIBPICO_INCLUDE_DIR} )

  add_definitions(-DSCOPE)
endif()

# add executables
file(GLOB tools [a-zA-Z]*.c[a-zA-Z]*)

foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  target_link_libraries(${execname} -lCOM -lPS -lMETER -lLOAD -lDevCom)
  add_dependencies(${execname} COM PS METER LOAD DevCom)

  if( ${ENABLE_SCOPE} )
    target_link_libraries(${execname} -lScope)
    add_dependencies(${execname} Scope)
  endif()

endforeach()
