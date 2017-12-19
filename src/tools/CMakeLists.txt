# Check for OpenCV
find_package( OpenCV QUIET )

if ( ${OpenCV_FOUND} )
  # add executables
  file(GLOB tools *.c*)
  foreach(target ${tools})
    get_filename_component(execname ${target} NAME_WE)
    get_filename_component(srcfile ${target} NAME)
    add_executable(${execname} ${srcfile})
    add_dependencies(${execname} libGPIB)
    set_target_properties(${execname} PROPERTIES LINKER_LANGUAGE CXX)
  endforeach()
else()
  message("Disabling tools due to missing OpenCV")
endif()
