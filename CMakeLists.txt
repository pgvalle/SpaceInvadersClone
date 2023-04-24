cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)
file(GLOB_RECURSE SOURCE_FILES src/*.c)
add_executable(SpaceInvadersClone ${SOURCE_FILES})

set_target_properties(
	SpaceInvadersClone  PROPERTIES
	C_STANDARD          11
	C_STANDARD_REQUIRED TRUE
	C_EXTENSIONS        FALSE
)

find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
	include(FetchContent)

	FetchContent_Declare(
		SDL2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.26.4
		SOURCE_DIR ext/SDL2
	)

	# pre-configuration and download
	set(SDL_STATIC_ENABLED_BY_DEFAULT OFF)
	set(SDL_TEST_ENABLED_BY_DEFAULT   OFF)
	FetchContent_MakeAvailable(SDL2)

	# integration
	add_dependencies(SpaceInvadersClone SDL2)
	add_custom_command(TARGET SpaceInvadersClone POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINARY_DIR}/SDL2.dll ${CMAKE_BINARY_DIR}
  )
endif()

target_include_directories(SpaceInvadersClone PRIVATE ${SDL2_INCLUDE_DIRS} ext/ src/)
target_link_libraries(SpaceInvadersClone PRIVATE SDL2main SDL2 m)

