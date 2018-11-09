# add global dependencies
include_directories( ../libDevCom )
include_directories( ../libGPIB/include )
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles PBv2TB.cpp
  PBv2Test.cpp
)

add_library(pbv2 SHARED ${SrcFiles}) 
target_link_libraries(pbv2 -lDevCom -lGPIB)

<<<<<<< HEAD
add_subdirectory(tools)
add_subdirectory(singleboard)
=======
# add executables
file(GLOB tools [a-zA-Z]*.c[a-zA-Z]*)

foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  add_dependencies(${execname} GPIB)
  target_link_libraries(${execname} -lGPIB)
  if ( ${ENABLE_SCOPE} )
    target_link_libraries(${execname} -lScope)
    add_dependencies(${execname} Scope)
  endif()
endforeach()
>>>>>>> origin
