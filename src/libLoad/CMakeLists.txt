#
# Prepare the library

# Add the headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libCom )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libUtils )

# Add all *.c* files as source code
set(SrcFiles
  Bk85xx.cpp
  TTILD400P.cpp)

add_library(LOAD SHARED ${SrcFiles})
add_dependencies(LOAD COM)
target_link_libraries(LOAD COM)
