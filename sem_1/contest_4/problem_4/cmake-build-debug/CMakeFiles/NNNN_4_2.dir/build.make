# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\CLionProjects\NNNN_4_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/NNNN_4_2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NNNN_4_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NNNN_4_2.dir/flags.make

CMakeFiles/NNNN_4_2.dir/main.cpp.obj: CMakeFiles/NNNN_4_2.dir/flags.make
CMakeFiles/NNNN_4_2.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NNNN_4_2.dir/main.cpp.obj"
	"C:\Program Files (x86)\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE"  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\NNNN_4_2.dir\main.cpp.obj -c C:\Users\User\CLionProjects\NNNN_4_2\main.cpp

CMakeFiles/NNNN_4_2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NNNN_4_2.dir/main.cpp.i"
	"C:\Program Files (x86)\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\User\CLionProjects\NNNN_4_2\main.cpp > CMakeFiles\NNNN_4_2.dir\main.cpp.i

CMakeFiles/NNNN_4_2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NNNN_4_2.dir/main.cpp.s"
	"C:\Program Files (x86)\MINGW-~1\I686-7~1.0-P\mingw32\bin\G__~1.EXE" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\User\CLionProjects\NNNN_4_2\main.cpp -o CMakeFiles\NNNN_4_2.dir\main.cpp.s

CMakeFiles/NNNN_4_2.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/NNNN_4_2.dir/main.cpp.obj.requires

CMakeFiles/NNNN_4_2.dir/main.cpp.obj.provides: CMakeFiles/NNNN_4_2.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\NNNN_4_2.dir\build.make CMakeFiles/NNNN_4_2.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/NNNN_4_2.dir/main.cpp.obj.provides

CMakeFiles/NNNN_4_2.dir/main.cpp.obj.provides.build: CMakeFiles/NNNN_4_2.dir/main.cpp.obj


# Object files for target NNNN_4_2
NNNN_4_2_OBJECTS = \
"CMakeFiles/NNNN_4_2.dir/main.cpp.obj"

# External object files for target NNNN_4_2
NNNN_4_2_EXTERNAL_OBJECTS =

NNNN_4_2.exe: CMakeFiles/NNNN_4_2.dir/main.cpp.obj
NNNN_4_2.exe: CMakeFiles/NNNN_4_2.dir/build.make
NNNN_4_2.exe: CMakeFiles/NNNN_4_2.dir/linklibs.rsp
NNNN_4_2.exe: CMakeFiles/NNNN_4_2.dir/objects1.rsp
NNNN_4_2.exe: CMakeFiles/NNNN_4_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NNNN_4_2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\NNNN_4_2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NNNN_4_2.dir/build: NNNN_4_2.exe

.PHONY : CMakeFiles/NNNN_4_2.dir/build

CMakeFiles/NNNN_4_2.dir/requires: CMakeFiles/NNNN_4_2.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/NNNN_4_2.dir/requires

CMakeFiles/NNNN_4_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\NNNN_4_2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/NNNN_4_2.dir/clean

CMakeFiles/NNNN_4_2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\User\CLionProjects\NNNN_4_2 C:\Users\User\CLionProjects\NNNN_4_2 C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug C:\Users\User\CLionProjects\NNNN_4_2\cmake-build-debug\CMakeFiles\NNNN_4_2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NNNN_4_2.dir/depend
