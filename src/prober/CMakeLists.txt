# add global dependencies
if ( OpenCV_FOUND )
  include_directories( ../libGalil/include )
  include_directories( ../libZaber/include )
  include_directories( ../libWaferProb/include )
  link_libraries( Galil )
  link_libraries( Zaber )
  link_libraries( WaferProb )

  if(APPLE)
    set(MACOSX_DEPLOYMENT_TARGET 10.10)
    link_libraries(/Applications/gclib/dylib/gclib.0.dylib /Applications/gclib/dylib/gclibo.0.dylib ${OpenCV_LIBS})
  else()
    link_libraries(gclib gclibo ${OpenCV_LIBS})
  endif()

  # add executables
  file(GLOB tools *.c*)
  foreach(target ${tools})
    get_filename_component(execname ${target} NAME_WE)
    get_filename_component(srcfile ${target} NAME)

    add_executable(${execname} ${srcfile})

    add_dependencies(${execname} libGalil libZaber libWaferProb)
  endforeach()
endif()
