# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tank/funris

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tank/funris/obj

# Include any dependencies generated for this target.
include src/server/collision/CMakeFiles/collision.dir/depend.make

# Include the progress variables for this target.
include src/server/collision/CMakeFiles/collision.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/collision/CMakeFiles/collision.dir/flags.make

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o: ../src/server/collision/Management/VMapFactory.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Management/VMapFactory.cpp.o -c /home/tank/funris/src/server/collision/Management/VMapFactory.cpp

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Management/VMapFactory.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Management/VMapFactory.cpp > CMakeFiles/collision.dir/Management/VMapFactory.cpp.i

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Management/VMapFactory.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Management/VMapFactory.cpp -o CMakeFiles/collision.dir/Management/VMapFactory.cpp.s

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o: ../src/server/collision/Management/VMapManager2.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Management/VMapManager2.cpp.o -c /home/tank/funris/src/server/collision/Management/VMapManager2.cpp

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Management/VMapManager2.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Management/VMapManager2.cpp > CMakeFiles/collision.dir/Management/VMapManager2.cpp.i

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Management/VMapManager2.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Management/VMapManager2.cpp -o CMakeFiles/collision.dir/Management/VMapManager2.cpp.s

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o: ../src/server/collision/Maps/TileAssembler.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o -c /home/tank/funris/src/server/collision/Maps/TileAssembler.cpp

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Maps/TileAssembler.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Maps/TileAssembler.cpp > CMakeFiles/collision.dir/Maps/TileAssembler.cpp.i

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Maps/TileAssembler.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Maps/TileAssembler.cpp -o CMakeFiles/collision.dir/Maps/TileAssembler.cpp.s

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o: ../src/server/collision/Maps/MapTree.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Maps/MapTree.cpp.o -c /home/tank/funris/src/server/collision/Maps/MapTree.cpp

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Maps/MapTree.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Maps/MapTree.cpp > CMakeFiles/collision.dir/Maps/MapTree.cpp.i

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Maps/MapTree.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Maps/MapTree.cpp -o CMakeFiles/collision.dir/Maps/MapTree.cpp.s

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o: ../src/server/collision/Models/ModelInstance.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Models/ModelInstance.cpp.o -c /home/tank/funris/src/server/collision/Models/ModelInstance.cpp

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Models/ModelInstance.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Models/ModelInstance.cpp > CMakeFiles/collision.dir/Models/ModelInstance.cpp.i

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Models/ModelInstance.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Models/ModelInstance.cpp -o CMakeFiles/collision.dir/Models/ModelInstance.cpp.s

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o: ../src/server/collision/Models/WorldModel.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/Models/WorldModel.cpp.o -c /home/tank/funris/src/server/collision/Models/WorldModel.cpp

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/Models/WorldModel.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/Models/WorldModel.cpp > CMakeFiles/collision.dir/Models/WorldModel.cpp.i

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/Models/WorldModel.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/Models/WorldModel.cpp -o CMakeFiles/collision.dir/Models/WorldModel.cpp.s

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.provides.build

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o: src/server/collision/CMakeFiles/collision.dir/flags.make
src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o: ../src/server/collision/BoundingIntervalHierarchy.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tank/funris/obj/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -o CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o -c /home/tank/funris/src/server/collision/BoundingIntervalHierarchy.cpp

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.i"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -E /home/tank/funris/src/server/collision/BoundingIntervalHierarchy.cpp > CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.i

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.s"
	cd /home/tank/funris/obj/src/server/collision && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include /home/tank/funris/obj/src/server/collision/collisionPCH.h -Winvalid-pch  -S /home/tank/funris/src/server/collision/BoundingIntervalHierarchy.cpp -o CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.s

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.requires:
.PHONY : src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.requires

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.provides: src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.requires
	$(MAKE) -f src/server/collision/CMakeFiles/collision.dir/build.make src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.provides.build
.PHONY : src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.provides

src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.provides.build: src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o
.PHONY : src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.provides.build

# Object files for target collision
collision_OBJECTS = \
"CMakeFiles/collision.dir/Management/VMapFactory.cpp.o" \
"CMakeFiles/collision.dir/Management/VMapManager2.cpp.o" \
"CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o" \
"CMakeFiles/collision.dir/Maps/MapTree.cpp.o" \
"CMakeFiles/collision.dir/Models/ModelInstance.cpp.o" \
"CMakeFiles/collision.dir/Models/WorldModel.cpp.o" \
"CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o"

# External object files for target collision
collision_EXTERNAL_OBJECTS =

src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/build.make
src/server/collision/libcollision.a: src/server/collision/CMakeFiles/collision.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libcollision.a"
	cd /home/tank/funris/obj/src/server/collision && $(CMAKE_COMMAND) -P CMakeFiles/collision.dir/cmake_clean_target.cmake
	cd /home/tank/funris/obj/src/server/collision && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/collision.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/collision/CMakeFiles/collision.dir/build: src/server/collision/libcollision.a
.PHONY : src/server/collision/CMakeFiles/collision.dir/build

src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Management/VMapFactory.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Management/VMapManager2.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Maps/TileAssembler.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Maps/MapTree.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Models/ModelInstance.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/Models/WorldModel.cpp.o.requires
src/server/collision/CMakeFiles/collision.dir/requires: src/server/collision/CMakeFiles/collision.dir/BoundingIntervalHierarchy.cpp.o.requires
.PHONY : src/server/collision/CMakeFiles/collision.dir/requires

src/server/collision/CMakeFiles/collision.dir/clean:
	cd /home/tank/funris/obj/src/server/collision && $(CMAKE_COMMAND) -P CMakeFiles/collision.dir/cmake_clean.cmake
.PHONY : src/server/collision/CMakeFiles/collision.dir/clean

src/server/collision/CMakeFiles/collision.dir/depend:
	cd /home/tank/funris/obj && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tank/funris /home/tank/funris/src/server/collision /home/tank/funris/obj /home/tank/funris/obj/src/server/collision /home/tank/funris/obj/src/server/collision/CMakeFiles/collision.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/server/collision/CMakeFiles/collision.dir/depend

