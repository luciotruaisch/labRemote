#
# Look for dependencies

# Find libpico
find_package(Pico)

if (NOT ${ENABLE_SCOPE} )
  message("Disabling libScope due to missing libpico")
  return()
endif()

#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Add all *.c* files as source code
set(SrcFiles PicoScope.cpp
  PS6000.cpp
)

include_directories( ${LIBPICO_INCLUDE_DIR} )
add_library(Scope SHARED ${SrcFiles}) 
target_link_libraries(Scope ${LIBPICO_LIBRARIES})
