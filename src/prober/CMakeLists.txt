# Check for OpenCV
find_package( OpenCV QUIET )

if ( ${libGalil_FOUND} AND ${libZaber_FOUND} AND ${libWaferProb_FOUND} )
  include_directories( ../libGalil/include )
  include_directories( ../libZaber/include )
  include_directories( ../libWaferProb/include )


  # add one exectable foreach 
  add_executable(eg_galil eg_galil.cpp)
  target_link_libraries(eg_galil Galil WaferProb)

  add_executable(eg_zaber eg_zaber.cpp)
  target_link_libraries(eg_zaber Zaber)

  add_executable(motion_controller motion_controller.cpp)
  target_link_libraries(motion_controller Galil Zaber WaferProb)

  include_directories(../libGPIB/include)
  add_executable(agilent_test agilent_test.cpp)
  target_link_libraries(agilent_test GPIB)

  add_executable(keithley2000_test keithley2000_test.cpp)
  target_link_libraries(keithley2000_test GPIB)

  add_executable(keithley_test2 keithley_test2.cpp)
  target_link_libraries(keithley_test2 GPIB)

  add_executable(power_on_wafer power_on_wafer.cpp)
  target_link_libraries(power_on_wafer GPIB)

  include_directories( ../libImageRec/include )
  add_executable(test_opencv_camera test_opencv_camera.cpp)
  target_link_libraries(test_opencv_camera ${OpenCV_LIBS})

  add_executable(test_sift test_sift.cpp)
  target_link_libraries(test_sift ImageRec ${OpenCV_LIBS})
else()
  message("Disabling prober due to missing dependencies")
  message(" libGalil = ${libGalil_FOUND}")
  message(" libZaber = ${libZaber_FOUND}")
  message(" libWaferProb = ${libWaferProb_FOUND}")
endif()
