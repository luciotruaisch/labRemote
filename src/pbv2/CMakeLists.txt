# add global dependencies
include_directories( ../libGPIB/include )

# preferred to use target_link_libraries
#link_libraries( GPIB )

# add executables
file(GLOB tools *.c*)
foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  target_link_libraries(${execname} GPIB)
  # seems reduntant
  #add_dependencies(${execname} libGPIB)
  
endforeach()
