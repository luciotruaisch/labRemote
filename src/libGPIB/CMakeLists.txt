#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
set(SrcFiles AgilentPs.cpp
  SorensenPs.cpp
  Bk85xx.cpp
  Keithley24XX.cpp
  Keithley2000.cpp
  MojoCom.cpp
  HP3478A.cpp
  RigolDP832.cpp
  TTIMX180TPPs.cpp
  TTITSX1820PPs.cpp
  SerialCom.cpp
  Tektronix654C.cpp)


add_library(GPIB SHARED ${SrcFiles}) 
