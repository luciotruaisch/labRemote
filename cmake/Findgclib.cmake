# - Try to find libmpsse
# Once done this will define
#  gclib_FOUND - System has libmpsse
#  gclib_INCLUDE_DIRS - The libmpsse include directories
#  gclib_LIBRARIES - The libraries needed to use libmpsse
#  gclib_DEFINITIONS - Compiler switches required for using libmpsse

FIND_PATH(gclib_INCLUDE_DIR gclib.h
          HINTS /usr/include)

FIND_LIBRARY(gclib_LIBRARY NAMES libgclib.so.0
              HINTS /usr/lib )

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set gclib_FOUND to TRUE
# if all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gclib DEFAULT_MSG
  gclib_LIBRARY gclib_INCLUDE_DIR)

MARK_AS_ADVANCED(gclib_INCLUDE_DIR gclib_LIBRARY )

SET(gclib_LIBRARIES ${gclib_LIBRARY} )
SET(gclib_INCLUDE_DIRS ${gclib_INCLUDE_DIR} )
