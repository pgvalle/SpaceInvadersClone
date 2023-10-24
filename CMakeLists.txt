cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)
file(GLOB_RECURSE SOURCE_FILES src/*.c)
add_executable(SpaceInvadersClone ${SOURCE_FILES})

set_target_properties(SpaceInvadersClone PROPERTIES
	C_STANDARD 11 C_STANDARD_REQUIRED TRUE C_EXTENSIONS FALSE)

find_package(SDL2 REQUIRED)
target_link_libraries(SpaceInvadersClone PRIVATE SDL2main SDL2 m)
target_include_directories(SpaceInvadersClone PRIVATE
	${SDL2_INCLUDE_DIRS} ext/ src/)
