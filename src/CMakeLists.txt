#
# Check for recent compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "GCC version must be at least 4.8!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
    message(FATAL_ERROR "Clang version must be at least 3.2!")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compile_options(-g)
else()
  add_compile_options(-O4)
endif()
add_compile_options(-Wfatal-errors)
add_compile_options(-Wno-deprecated)
add_compile_options(-std=c++11)

#
# Add libraries
add_subdirectory(libGPIB)
add_subdirectory(libGalil)
add_subdirectory(libImageRec)
add_subdirectory(libWaferProb)
add_subdirectory(libZaber)

#
# Add binaries
add_subdirectory(pbv2)
add_subdirectory(prober)
