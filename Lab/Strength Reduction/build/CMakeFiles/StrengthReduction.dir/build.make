# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build"

# Include any dependencies generated for this target.
include CMakeFiles/StrengthReduction.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/StrengthReduction.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/StrengthReduction.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/StrengthReduction.dir/flags.make

CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o: CMakeFiles/StrengthReduction.dir/flags.make
CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o: ../StrengthReduction.cpp
CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o: CMakeFiles/StrengthReduction.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o -MF CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o.d -o CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o -c "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/StrengthReduction.cpp"

CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/StrengthReduction.cpp" > CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.i

CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/StrengthReduction.cpp" -o CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.s

# Object files for target StrengthReduction
StrengthReduction_OBJECTS = \
"CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o"

# External object files for target StrengthReduction
StrengthReduction_EXTERNAL_OBJECTS =

libStrengthReduction.so: CMakeFiles/StrengthReduction.dir/StrengthReduction.cpp.o
libStrengthReduction.so: CMakeFiles/StrengthReduction.dir/build.make
libStrengthReduction.so: CMakeFiles/StrengthReduction.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libStrengthReduction.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/StrengthReduction.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/StrengthReduction.dir/build: libStrengthReduction.so
.PHONY : CMakeFiles/StrengthReduction.dir/build

CMakeFiles/StrengthReduction.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/StrengthReduction.dir/cmake_clean.cmake
.PHONY : CMakeFiles/StrengthReduction.dir/clean

CMakeFiles/StrengthReduction.dir/depend:
	cd "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction" "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction" "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build" "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build" "/home/frt/compilatori/Compilatori-2024-2025-Frattolillo-Canovi/Lab/Strength Reduction/build/CMakeFiles/StrengthReduction.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/StrengthReduction.dir/depend

