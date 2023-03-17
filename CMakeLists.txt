cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)
file(GLOB_RECURSE SOURCE_FILES src/*.h src/*.c)
add_executable(SpaceInvadersClone ${SOURCE_FILES})

set_target_properties(
	SpaceInvadersClone PROPERTIES
	C_STANDARD          99
	C_STANDARD_REQUIRED TRUE
	C_EXTENSIONS        FALSE
)

if(WIN32)
	include(FetchContent)

	# downloading SDL2
	FetchContent_Declare(
		SDL2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.26.4
		SOURCE_DIR ext/SDL2
	)

	# configuring SDL2 build
	set(SDL_STATIC_ENABLED_BY_DEFAULT OFF)
	set(SDL_TEST_ENABLED_BY_DEFAULT   OFF)

	# integrating SDL2 with project
	FetchContent_MakeAvailable(SDL2)
	FetchContent_GetProperties(SDL2)
	add_dependencies(SpaceInvadersClone SDL2)
	target_include_directories(SpaceInvadersClone PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(SpaceInvadersClone PRIVATE SDL2main SDL2)

	add_custom_command(TARGET SpaceInvadersClone POST_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_BINARY_DIR}/SDL2d.dll ${CMAKE_BINARY_DIR}
	)
else()
	find_package(SDL2 REQUIRED)
endif()
