# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/stecanovi/workspace/Laboratorio_Compilatori/Assign

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build

# Include any dependencies generated for this target.
include CMakeFiles/AlgebraicIdentity.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/AlgebraicIdentity.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/AlgebraicIdentity.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AlgebraicIdentity.dir/flags.make

CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o: CMakeFiles/AlgebraicIdentity.dir/flags.make
CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o: /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/AlgebraicIdentity.cpp
CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o: CMakeFiles/AlgebraicIdentity.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o -MF CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o.d -o CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o -c /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/AlgebraicIdentity.cpp

CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/AlgebraicIdentity.cpp > CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.i

CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/AlgebraicIdentity.cpp -o CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.s

# Object files for target AlgebraicIdentity
AlgebraicIdentity_OBJECTS = \
"CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o"

# External object files for target AlgebraicIdentity
AlgebraicIdentity_EXTERNAL_OBJECTS =

libAlgebraicIdentity.so: CMakeFiles/AlgebraicIdentity.dir/AlgebraicIdentity.cpp.o
libAlgebraicIdentity.so: CMakeFiles/AlgebraicIdentity.dir/build.make
libAlgebraicIdentity.so: CMakeFiles/AlgebraicIdentity.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libAlgebraicIdentity.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AlgebraicIdentity.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AlgebraicIdentity.dir/build: libAlgebraicIdentity.so
.PHONY : CMakeFiles/AlgebraicIdentity.dir/build

CMakeFiles/AlgebraicIdentity.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AlgebraicIdentity.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AlgebraicIdentity.dir/clean

CMakeFiles/AlgebraicIdentity.dir/depend:
	cd /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stecanovi/workspace/Laboratorio_Compilatori/Assign /home/stecanovi/workspace/Laboratorio_Compilatori/Assign /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build /home/stecanovi/workspace/Laboratorio_Compilatori/Assign/build/CMakeFiles/AlgebraicIdentity.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/AlgebraicIdentity.dir/depend

