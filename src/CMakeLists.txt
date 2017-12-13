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

#
# Add binaries
add_subdirectory(pbv2)

# # add executables
# file(GLOB tools tools/*.c*)
# foreach(target ${tools})
# get_filename_component(execname ${target} NAME_WE)
# get_filename_component(srcfile ${target} NAME)
# add_executable(${execname} tools/${srcfile})
# add_dependencies(${execname} labRemote)
# set_target_properties(${execname} PROPERTIES LINKER_LANGUAGE CXX)


# if(APPLE)
#   set(MACOSX_DEPLOYMENT_TARGET 10.10)
#   target_link_libraries(${execname} labRemote /Applications/gclib/dylib/gclib.0.dylib /Applications/gclib/dylib/gclibo.0.dylib ${OpenCV_LIBS})
#   # target_link_libraries(${execname} labRemote /Applications/gclib/dylib/gclib.0.dylib /Applications/gclib/dylib/gclibo.0.dylib)
# else()
#   target_link_libraries(${execname} labRemote gclib gclibo ${OpenCV_LIBS})
#   # target_link_libraries(${execname} labRemote gclib gclibo)
# endif()

# endforeach()
