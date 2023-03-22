cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)
file(GLOB_RECURSE SOURCE_FILES src/*.c)
add_executable(SpaceInvadersClone ${SOURCE_FILES})

set_target_properties(
	SpaceInvadersClone PROPERTIES
	C_STANDARD          99
	C_STANDARD_REQUIRED TRUE
	C_EXTENSIONS        FALSE
)

find_package(SDL2 QUIET)
if(SDL2_FOUND)

else()
	include(FetchContent)

	FetchContent_Declare(
		SDL2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.26.4
		SOURCE_DIR ext/SDL2
	)

	set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
	set(SDL_TEST_ENABLED_BY_DEFAULT   OFF)
	
	FetchContent_MakeAvailable(SDL2)
	FetchContent_GetProperties(SDL2)

	# integrating SDL2 with project
	add_dependencies(SpaceInvadersClone SDL2-static)
	target_include_directories(SpaceInvadersClone PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(SpaceInvadersClone PRIVATE SDL2main SDL2-static)
endif()
