# Add the headers
include_directories( ./ )

# Add all *.c* files as source code
file(GLOB SrcFiles *.c*)
add_library(Zaber SHARED ${SrcFiles})

# Tell rest of labRemote that the library exists
set(libZaber_FOUND TRUE PARENT_SCOPE)
