# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build

# Include any dependencies generated for this target.
include CMakeFiles/imac3_td_dg_TD1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/imac3_td_dg_TD1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imac3_td_dg_TD1.dir/flags.make

CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o: CMakeFiles/imac3_td_dg_TD1.dir/flags.make
CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o: ../source/td1/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o -c /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/source/td1/main.cpp

CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/source/td1/main.cpp > CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.i

CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/source/td1/main.cpp -o CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.s

# Object files for target imac3_td_dg_TD1
imac3_td_dg_TD1_OBJECTS = \
"CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o"

# External object files for target imac3_td_dg_TD1
imac3_td_dg_TD1_EXTERNAL_OBJECTS =

imac3_td_dg_TD1: CMakeFiles/imac3_td_dg_TD1.dir/source/td1/main.cpp.o
imac3_td_dg_TD1: CMakeFiles/imac3_td_dg_TD1.dir/build.make
imac3_td_dg_TD1: CMakeFiles/imac3_td_dg_TD1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable imac3_td_dg_TD1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imac3_td_dg_TD1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imac3_td_dg_TD1.dir/build: imac3_td_dg_TD1

.PHONY : CMakeFiles/imac3_td_dg_TD1.dir/build

CMakeFiles/imac3_td_dg_TD1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imac3_td_dg_TD1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imac3_td_dg_TD1.dir/clean

CMakeFiles/imac3_td_dg_TD1.dir/depend:
	cd /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build /home/binabik/Documents/projets_gite/imac/imac3_digital_geometry/build/CMakeFiles/imac3_td_dg_TD1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imac3_td_dg_TD1.dir/depend
