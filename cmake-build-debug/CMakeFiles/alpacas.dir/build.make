# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Develop\Projects\Alpaca-s-Head

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/alpacas.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/alpacas.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/alpacas.dir/flags.make

CMakeFiles/alpacas.dir/src/main.cpp.obj: CMakeFiles/alpacas.dir/flags.make
CMakeFiles/alpacas.dir/src/main.cpp.obj: CMakeFiles/alpacas.dir/includes_CXX.rsp
CMakeFiles/alpacas.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/alpacas.dir/src/main.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\alpacas.dir\src\main.cpp.obj -c E:\Develop\Projects\Alpaca-s-Head\src\main.cpp

CMakeFiles/alpacas.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alpacas.dir/src/main.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Develop\Projects\Alpaca-s-Head\src\main.cpp > CMakeFiles\alpacas.dir\src\main.cpp.i

CMakeFiles/alpacas.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alpacas.dir/src/main.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Develop\Projects\Alpaca-s-Head\src\main.cpp -o CMakeFiles\alpacas.dir\src\main.cpp.s

# Object files for target alpacas
alpacas_OBJECTS = \
"CMakeFiles/alpacas.dir/src/main.cpp.obj"

# External object files for target alpacas
alpacas_EXTERNAL_OBJECTS =

alpacas.exe: CMakeFiles/alpacas.dir/src/main.cpp.obj
alpacas.exe: CMakeFiles/alpacas.dir/build.make
alpacas.exe: libGame.a
alpacas.exe: libBase.a
alpacas.exe: C:/msys64/mingw64/lib/libSDL2main.a
alpacas.exe: C:/msys64/mingw64/lib/libSDL2.dll.a
alpacas.exe: C:/msys64/mingw64/lib/libSDL2_image.dll.a
alpacas.exe: C:/msys64/mingw64/lib/libSDL2_ttf.dll.a
alpacas.exe: C:/msys64/mingw64/lib/libSDL2_mixer.dll.a
alpacas.exe: C:/msys64/mingw64/lib/libglew32.dll.a
alpacas.exe: CMakeFiles/alpacas.dir/linklibs.rsp
alpacas.exe: CMakeFiles/alpacas.dir/objects1.rsp
alpacas.exe: CMakeFiles/alpacas.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable alpacas.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\alpacas.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/alpacas.dir/build: alpacas.exe

.PHONY : CMakeFiles/alpacas.dir/build

CMakeFiles/alpacas.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\alpacas.dir\cmake_clean.cmake
.PHONY : CMakeFiles/alpacas.dir/clean

CMakeFiles/alpacas.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Develop\Projects\Alpaca-s-Head E:\Develop\Projects\Alpaca-s-Head E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug E:\Develop\Projects\Alpaca-s-Head\cmake-build-debug\CMakeFiles\alpacas.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/alpacas.dir/depend

