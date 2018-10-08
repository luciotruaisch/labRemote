# Check for OpenCV
find_package( OpenCV QUIET )

if ( ${libGalil_FOUND} AND ${libZaber_FOUND} AND ${libWaferProb_FOUND} )
  include_directories( ../libGalil/include )
  include_directories( ../libZaber/include )
  include_directories( ../libWaferProb/include )


include_directories(../libGPIB/include)
include_directories( ../libImageRec/include )

link_directories( ${CMAKE_BINARY_DIR}/lib )
file(GLOB tools [a-zA-Z]*.c[a-zA-Z]*)

foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)

  add_executable(${execname} ${srcfile})
  #target_link_libraries(${execname} -lGPIB)
endforeach()


  # target link libraries for each 
  target_link_libraries(eg_galil Galil WaferProb)

  target_link_libraries(eg_zaber Zaber)

  target_link_libraries(motion_controller Galil Zaber WaferProb)

  target_link_libraries(agilent_test GPIB)

  target_link_libraries(keithley2000_test GPIB)

  #target_link_libraries(keithley_test2 GPIB)

  target_link_libraries(power_on_wafer GPIB)

  target_link_libraries(hp_test GPIB)

  target_link_libraries(test_opencv_camera ${OpenCV_LIBS})

  target_link_libraries(test_sift ImageRec ${OpenCV_LIBS})

else()
  message("Disabling prober due to missing dependencies")
  message(" libGalil = ${libGalil_FOUND}")
  message(" libZaber = ${libZaber_FOUND}")
  message(" libWaferProb = ${libWaferProb_FOUND}")
endif()
