# - Try to find libmpsse
# Once done this will define
#  LIBLIBFTDI_FOUND - System has libmpsse
#  LIBLIBFTDI_INCLUDE_DIRS - The libmpsse include directories
#  LIBLIBFTDI_LIBRARIES - The libraries needed to use libmpsse
#  LIBLIBFTDI_DEFINITIONS - Compiler switches required for using libmpsse

FIND_PATH(LIBFTDI_INCLUDE_DIR ftdi.h
  HINTS /usr/include/libftdi1 )

FIND_LIBRARY(LIBFTDI_LIBRARY NAMES ftdi1 libftdi1
  HINTS /usr/lib64 )

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBFTDI_FOUND to TRUE
# if all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libftdi  DEFAULT_MSG
  LIBFTDI_LIBRARY LIBFTDI_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBFTDI_INCLUDE_DIR LIBFTDI_LIBRARY )

SET(LIBFTDI_LIBRARIES ${LIBFTDI_LIBRARY} )
SET(LIBFTDI_INCLUDE_DIRS ${LIBFTDI_INCLUDE_DIR} )
