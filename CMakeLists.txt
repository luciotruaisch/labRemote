cmake_minimum_required (VERSION 2.8)

project (labRemote)

# The version number.
set (labRemote_VERSION_MAJOR 1)
set (labRemote_VERSION_MINOR 0)

# Nicer structure for binary files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MACOSX_RPATH 1)
# Source code
add_subdirectory(src)
