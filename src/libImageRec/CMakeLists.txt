# Check for OpenCV
find_package( OpenCV QUIET )

if ( ${OpenCV_FOUND} )
  list(FIND OpenCV_LIB_COMPONENTS "opencv_xfeatures2d" OpenCV_xfeatures2d_FOUND)
  if( ${OpenCV_xfeatures2d_FOUND} EQUAL -1 )
    set(OpenCV_xfeatures2d_FOUND FALSE)
  else()
    set(OpenCV_xfeatures2d_FOUND TRUE)
  endif()
else()
  set(OpenCV_xfeatures2d_FOUND FALSE)
endif()

if ( ${OpenCV_FOUND} AND ${OpenCV_xfeatures2d_FOUND} )
  # Add the headers
  include_directories( include )

  # Add all *.c* files as source code
  file(GLOB SrcFiles *.c*)
  add_library(ImageRec SHARED ${SrcFiles}) 
  target_link_libraries(ImageRec ${OpenCV_LIBS})

  # Tell rest of labRemote that the library exists
  set(libImageRec_FOUND TRUE)
else()
  message("Disabling libImageRec due to missing dependencies")
  message(" OpenCV = ${OpenCV_FOUND}")
  message(" OpenCV-xfeatures2d = ${OpenCV_xfeatures2d_FOUND}")

  set(libImageRec_FOUND FALSE)
endif()
