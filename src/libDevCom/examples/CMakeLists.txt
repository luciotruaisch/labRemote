# add global dependencies
include_directories( ../../libDevCom )
link_directories( ${CMAKE_BINARY_DIR}/lib )

# add executables
file(GLOB examples [a-zA-Z]*.c[a-zA-Z]*)

foreach(target ${examples})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  target_link_libraries(${execname} -lDevCom)
  add_dependencies(${execname} DevCom)
endforeach()
