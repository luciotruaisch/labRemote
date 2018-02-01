find_package( gclib QUIET )

if ( ${gclib_FOUND} )
  # Add the headers
  include_directories( include )

  # Add all *.c* files as source code
  file(GLOB SrcFiles *.c*)
  add_library(Galil SHARED ${SrcFiles}) 
  if(APPLE)
    target_link_libraries(Galil /Applications/gclib/dylib/gclib.0.dylib /Applications/gclib/dylib/gclibo.0.dylib)
  else()
    target_link_libraries(Galil gclib gclibo)
  endif()

  # Tell rest of labRemote that the library exists
  set(libGalil_FOUND TRUE PARENT_SCOPE)
else()
  message("Disabling libGalil due to missing dependencies")
  message(" gclib = ${gclib_FOUND}")

  set(libGalil_FOUND FALSE PARENT_SCOPE)
endif()
