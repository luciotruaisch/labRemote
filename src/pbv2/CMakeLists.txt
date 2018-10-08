# add global dependencies
#include_directories( ../libDevCom )
include_directories(../libDevCom/include)
link_directories( ${CMAKE_BINARY_DIR}/lib )

set(SrcFiles PBv2TB.cpp
)

add_library(pbv2 SHARED ${SrcFiles}) 

add_subdirectory(tools)
add_subdirectory(singleboard)

foreach(target ${pbv2})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  target_link_libraries(${execname} -lDevCom)
endforeach()
