# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/clemensv/Github/sbs1-eventhubs-bridge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/clemensv/Github/sbs1-eventhubs-bridge

# Include any dependencies generated for this target.
include CMakeFiles/sbs1-eventhub-bridge.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sbs1-eventhub-bridge.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sbs1-eventhub-bridge.dir/flags.make

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o: CMakeFiles/sbs1-eventhub-bridge.dir/flags.make
CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o: src/bridge/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/clemensv/Github/sbs1-eventhubs-bridge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o   -c /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/main.c

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/main.c > CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.i

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/main.c -o CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.s

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.requires:

.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.requires

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.provides: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.requires
	$(MAKE) -f CMakeFiles/sbs1-eventhub-bridge.dir/build.make CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.provides.build
.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.provides

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.provides.build: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o


CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o: CMakeFiles/sbs1-eventhub-bridge.dir/flags.make
CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o: src/bridge/sbs1_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/clemensv/Github/sbs1-eventhubs-bridge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o   -c /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/sbs1_client.c

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/sbs1_client.c > CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.i

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /Users/clemensv/Github/sbs1-eventhubs-bridge/src/bridge/sbs1_client.c -o CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.s

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.requires:

.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.requires

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.provides: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.requires
	$(MAKE) -f CMakeFiles/sbs1-eventhub-bridge.dir/build.make CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.provides.build
.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.provides

CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.provides.build: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o


# Object files for target sbs1-eventhub-bridge
sbs1__eventhub__bridge_OBJECTS = \
"CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o" \
"CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o"

# External object files for target sbs1-eventhub-bridge
sbs1__eventhub__bridge_EXTERNAL_OBJECTS =

sbs1-eventhub-bridge: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o
sbs1-eventhub-bridge: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o
sbs1-eventhub-bridge: CMakeFiles/sbs1-eventhub-bridge.dir/build.make
sbs1-eventhub-bridge: CMakeFiles/sbs1-eventhub-bridge.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/clemensv/Github/sbs1-eventhubs-bridge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable sbs1-eventhub-bridge"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sbs1-eventhub-bridge.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sbs1-eventhub-bridge.dir/build: sbs1-eventhub-bridge

.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/build

CMakeFiles/sbs1-eventhub-bridge.dir/requires: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/main.c.o.requires
CMakeFiles/sbs1-eventhub-bridge.dir/requires: CMakeFiles/sbs1-eventhub-bridge.dir/src/bridge/sbs1_client.c.o.requires

.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/requires

CMakeFiles/sbs1-eventhub-bridge.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sbs1-eventhub-bridge.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/clean

CMakeFiles/sbs1-eventhub-bridge.dir/depend:
	cd /Users/clemensv/Github/sbs1-eventhubs-bridge && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/clemensv/Github/sbs1-eventhubs-bridge /Users/clemensv/Github/sbs1-eventhubs-bridge /Users/clemensv/Github/sbs1-eventhubs-bridge /Users/clemensv/Github/sbs1-eventhubs-bridge /Users/clemensv/Github/sbs1-eventhubs-bridge/CMakeFiles/sbs1-eventhub-bridge.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sbs1-eventhub-bridge.dir/depend
