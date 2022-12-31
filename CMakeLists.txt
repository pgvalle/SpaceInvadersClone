cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)

file(GLOB_RECURSE PROJECT_SOURCE_FILES src/**.c)
file(GLOB_RECURSE PROJECT_HEADER_FILES src/**.h)

add_executable(SpaceInvadersClone ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES})

set_property(TARGET SpaceInvadersClone PROPERTY C_STANDARD 17)

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
