cmake_minimum_required(VERSION 3.12)

project(SpaceInvadersClone)
add_executable(SpaceInvadersClone stb_ds.h main.c)

set_target_properties(
	SpaceInvadersClone PROPERTIES
	C_STANDARD          99
	C_STANDARD_REQUIRED TRUE
	C_EXTENSIONS        FALSE
)

find_package(SDL2       REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(
	SpaceInvadersClone
	SDL2::SDL2main
	SDL2::SDL2
	SDL2_image::SDL2_image
)
