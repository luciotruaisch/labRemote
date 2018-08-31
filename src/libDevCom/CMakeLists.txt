# Commented files are from amacv2_tester version of the library. They still need to be
# ported to labRemote.
set(SrcFiles DeviceCom.cpp
  #UIOCom.cpp
  #SPICom.cpp
  #EndeavourCom.cpp
  #EndeavourRaw.cpp

  I2CCom.cpp
  I2CDevCom.cpp
  PCA9548ACom.cpp
  
  #FreqMeas.cpp

  ComIOException.cpp
  NotSupportedException.cpp

  #ClimateSensor.cpp
  #BME280.cpp

  DACDevice.cpp
  DAC5574.cpp
  #LTC2666.cpp

  ADCDevice.cpp
  AD799X.cpp
  #LTC2333.cpp

  #AD5160.cpp
)

add_library(DevCom SHARED ${SrcFiles}) 
