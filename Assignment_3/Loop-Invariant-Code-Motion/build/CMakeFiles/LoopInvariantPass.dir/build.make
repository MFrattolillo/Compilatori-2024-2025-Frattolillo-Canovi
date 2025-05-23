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
CMAKE_SOURCE_DIR = /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build

# Include any dependencies generated for this target.
include CMakeFiles/LoopInvariantPass.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/LoopInvariantPass.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/LoopInvariantPass.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LoopInvariantPass.dir/flags.make

CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o: CMakeFiles/LoopInvariantPass.dir/flags.make
CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o: /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/LoopInvariantPass.cpp
CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o: CMakeFiles/LoopInvariantPass.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o -MF CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o.d -o CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o -c /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/LoopInvariantPass.cpp

CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/LoopInvariantPass.cpp > CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.i

CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/LoopInvariantPass.cpp -o CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.s

# Object files for target LoopInvariantPass
LoopInvariantPass_OBJECTS = \
"CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o"

# External object files for target LoopInvariantPass
LoopInvariantPass_EXTERNAL_OBJECTS =

libLoopInvariantPass.so: CMakeFiles/LoopInvariantPass.dir/LoopInvariantPass.cpp.o
libLoopInvariantPass.so: CMakeFiles/LoopInvariantPass.dir/build.make
libLoopInvariantPass.so: CMakeFiles/LoopInvariantPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libLoopInvariantPass.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LoopInvariantPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LoopInvariantPass.dir/build: libLoopInvariantPass.so
.PHONY : CMakeFiles/LoopInvariantPass.dir/build

CMakeFiles/LoopInvariantPass.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LoopInvariantPass.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LoopInvariantPass.dir/clean

CMakeFiles/LoopInvariantPass.dir/depend:
	cd /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build /home/stecanovi/workspace/Laboratorio_Compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Assignment_3/Loop-Invariant-Code-Motion/build/CMakeFiles/LoopInvariantPass.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/LoopInvariantPass.dir/depend

