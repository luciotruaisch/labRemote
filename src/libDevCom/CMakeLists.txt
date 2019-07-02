# Commented files are from amacv2_tester version of the library. They still need to be
# ported to labRemote.
set(SrcFiles DeviceCom.cpp
  #UIOCom.cpp
  #SPICom.cpp

  I2CCom.cpp
  I2CDevCom.cpp
  PCA9548ACom.cpp

  SPICom.cpp

  ComIOException.cpp
  NotSupportedException.cpp

  ClimateSensor.cpp
  #BME280.cpp
  HIH6130.cpp

  DACDevice.cpp
  DAC5574.cpp
  MCP4801.cpp
  #LTC2666.cpp

  ADCDevice.cpp
  AD799X.cpp
  MAX11619.cpp
  MCP3428.cpp
  #LTC2333.cpp

  #AD5160.cpp

  AMAC.cpp
  AMAC_calibrate.cpp

  DeviceCalibration.cpp
  DummyCalibration.cpp
  LinearCalibration.cpp
  LineCalibration.cpp
  GainCalibration.cpp
  FileCalibration.cpp
)

if ( ${ENABLE_FTDI} )
  set(SrcFiles ${SrcFiles}
    I2CFTDICom.cpp)

  include_directories ( ${LIBFTDI_INCLUDE_DIR} ${LIBMPSSE_INCLUDE_DIR} )
else()
  message("skipping FTDICom due to missing libftdi1 or libmpsse")
  message(" libftdi1 = ${LIBFTDI_FOUND}")
  message(" libmpsse = ${LIBMPSSE_FOUND}")
endif()

add_library(DevCom SHARED ${SrcFiles})
if ( ${ENABLE_FTDI} )
  target_link_libraries(DevCom ${LIBMPSSE_LIBRARIES} ${LIBFTDI_LIBRARIES})
endif()
add_compile_options(-std=c++11)
