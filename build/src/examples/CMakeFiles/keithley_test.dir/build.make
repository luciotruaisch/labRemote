# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ltruaisch/labRemote

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ltruaisch/labRemote/build

# Include any dependencies generated for this target.
include src/examples/CMakeFiles/keithley_test.dir/depend.make

# Include the progress variables for this target.
include src/examples/CMakeFiles/keithley_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/examples/CMakeFiles/keithley_test.dir/flags.make

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o: src/examples/CMakeFiles/keithley_test.dir/flags.make
src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o: ../src/examples/keithley_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ltruaisch/labRemote/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o"
	cd /home/ltruaisch/labRemote/build/src/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/keithley_test.dir/keithley_test.cpp.o -c /home/ltruaisch/labRemote/src/examples/keithley_test.cpp

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/keithley_test.dir/keithley_test.cpp.i"
	cd /home/ltruaisch/labRemote/build/src/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ltruaisch/labRemote/src/examples/keithley_test.cpp > CMakeFiles/keithley_test.dir/keithley_test.cpp.i

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/keithley_test.dir/keithley_test.cpp.s"
	cd /home/ltruaisch/labRemote/build/src/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ltruaisch/labRemote/src/examples/keithley_test.cpp -o CMakeFiles/keithley_test.dir/keithley_test.cpp.s

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.requires:

.PHONY : src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.requires

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.provides: src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.requires
	$(MAKE) -f src/examples/CMakeFiles/keithley_test.dir/build.make src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.provides.build
.PHONY : src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.provides

src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.provides.build: src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o


# Object files for target keithley_test
keithley_test_OBJECTS = \
"CMakeFiles/keithley_test.dir/keithley_test.cpp.o"

# External object files for target keithley_test
keithley_test_EXTERNAL_OBJECTS =

bin/keithley_test: src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o
bin/keithley_test: src/examples/CMakeFiles/keithley_test.dir/build.make
bin/keithley_test: src/examples/CMakeFiles/keithley_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ltruaisch/labRemote/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/keithley_test"
	cd /home/ltruaisch/labRemote/build/src/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/keithley_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/examples/CMakeFiles/keithley_test.dir/build: bin/keithley_test

.PHONY : src/examples/CMakeFiles/keithley_test.dir/build

src/examples/CMakeFiles/keithley_test.dir/requires: src/examples/CMakeFiles/keithley_test.dir/keithley_test.cpp.o.requires

.PHONY : src/examples/CMakeFiles/keithley_test.dir/requires

src/examples/CMakeFiles/keithley_test.dir/clean:
	cd /home/ltruaisch/labRemote/build/src/examples && $(CMAKE_COMMAND) -P CMakeFiles/keithley_test.dir/cmake_clean.cmake
.PHONY : src/examples/CMakeFiles/keithley_test.dir/clean

src/examples/CMakeFiles/keithley_test.dir/depend:
	cd /home/ltruaisch/labRemote/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ltruaisch/labRemote /home/ltruaisch/labRemote/src/examples /home/ltruaisch/labRemote/build /home/ltruaisch/labRemote/build/src/examples /home/ltruaisch/labRemote/build/src/examples/CMakeFiles/keithley_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/examples/CMakeFiles/keithley_test.dir/depend

