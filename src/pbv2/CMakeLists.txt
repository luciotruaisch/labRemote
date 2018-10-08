# add global dependencies
include_directories( ../libGPIB/include )
include_directories( ../libDevCom )
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
  target_link_libraries(${execname} -lGPIB)
  target_link_libraries(${execname} -lDevCom)
  if ( ${ENABLE_SCOPE} )
    target_link_libraries(${execname} -lScope)
  endif()
endforeach()
