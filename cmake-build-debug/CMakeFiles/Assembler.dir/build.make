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
CMAKE_COMMAND = "C:\mingw64\CLion 2018.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\mingw64\CLion 2018.2.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\mingw64\CLionProjects\Assembler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\mingw64\CLionProjects\Assembler\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Assembler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Assembler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Assembler.dir/flags.make

CMakeFiles/Assembler.dir/ASM.c.obj: CMakeFiles/Assembler.dir/flags.make
CMakeFiles/Assembler.dir/ASM.c.obj: ASM.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\mingw64\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Assembler.dir/ASM.c.obj"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Assembler.dir\ASM.c.obj   -c C:\mingw64\CLionProjects\Assembler\cmake-build-debug\ASM.c

CMakeFiles/Assembler.dir/ASM.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Assembler.dir/ASM.c.i"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\mingw64\CLionProjects\Assembler\cmake-build-debug\ASM.c > CMakeFiles\Assembler.dir\ASM.c.i

CMakeFiles/Assembler.dir/ASM.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Assembler.dir/ASM.c.s"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\mingw64\CLionProjects\Assembler\cmake-build-debug\ASM.c -o CMakeFiles\Assembler.dir\ASM.c.s

CMakeFiles/Assembler.dir/mainFunctions.c.obj: CMakeFiles/Assembler.dir/flags.make
CMakeFiles/Assembler.dir/mainFunctions.c.obj: mainFunctions.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\mingw64\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Assembler.dir/mainFunctions.c.obj"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Assembler.dir\mainFunctions.c.obj   -c C:\mingw64\CLionProjects\Assembler\cmake-build-debug\mainFunctions.c

CMakeFiles/Assembler.dir/mainFunctions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Assembler.dir/mainFunctions.c.i"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\mingw64\CLionProjects\Assembler\cmake-build-debug\mainFunctions.c > CMakeFiles\Assembler.dir\mainFunctions.c.i

CMakeFiles/Assembler.dir/mainFunctions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Assembler.dir/mainFunctions.c.s"
	C:\mingw64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\mingw64\CLionProjects\Assembler\cmake-build-debug\mainFunctions.c -o CMakeFiles\Assembler.dir\mainFunctions.c.s

# Object files for target Assembler
Assembler_OBJECTS = \
"CMakeFiles/Assembler.dir/ASM.c.obj" \
"CMakeFiles/Assembler.dir/mainFunctions.c.obj"

# External object files for target Assembler
Assembler_EXTERNAL_OBJECTS =

Assembler.exe: CMakeFiles/Assembler.dir/ASM.c.obj
Assembler.exe: CMakeFiles/Assembler.dir/mainFunctions.c.obj
Assembler.exe: CMakeFiles/Assembler.dir/build.make
Assembler.exe: CMakeFiles/Assembler.dir/linklibs.rsp
Assembler.exe: CMakeFiles/Assembler.dir/objects1.rsp
Assembler.exe: CMakeFiles/Assembler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\mingw64\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Assembler.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Assembler.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Assembler.dir/build: Assembler.exe

.PHONY : CMakeFiles/Assembler.dir/build

CMakeFiles/Assembler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Assembler.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Assembler.dir/clean

CMakeFiles/Assembler.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\mingw64\CLionProjects\Assembler C:\mingw64\CLionProjects\Assembler C:\mingw64\CLionProjects\Assembler\cmake-build-debug C:\mingw64\CLionProjects\Assembler\cmake-build-debug C:\mingw64\CLionProjects\Assembler\cmake-build-debug\CMakeFiles\Assembler.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Assembler.dir/depend
