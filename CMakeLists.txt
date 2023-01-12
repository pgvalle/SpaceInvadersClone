cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)

file(GLOB_RECURSE SOURCE_FILES src/**.c)
file(GLOB_RECURSE HEADER_FILES src/**.h)

add_executable(SpaceInvadersClone ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(
	SpaceInvadersClone PROPERTIES
	C_STANDARD          99
	C_STANDARD_REQUIRED TRUE
	C_EXTENSIONS        FALSE
)

target_include_directories(
	SpaceInvadersClone PRIVATE
	${CMAKE_SOURCE_DIR}/src
)

find_package(SDL2       REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf   REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(
	SpaceInvadersClone
	SDL2::SDL2main
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	SDL2_mixer::SDL2_mixer
)
