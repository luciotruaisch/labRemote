# add global dependencies
include_directories( ../libGPIB/include )
link_libraries( GPIB )

# add executables
file(GLOB tools *.c*)
foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})

  add_dependencies(${execname} libGPIB)

endforeach()
