cmake_minimum_required(VERSION 3.22)

project(SpaceInvadersClone LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
#pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
# ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_MIXER_LIBRARY_DIRS} ${SDL2_MIXER_LIBRARIES}

include_directories(src/ ext/ ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_TTF_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS})

# Your executable
file(GLOB_RECURSE SRC_FILES src/**.cpp)
add_executable(sic ${SRC_FILES} ext/SDL_FontCache.c)

target_link_libraries(sic ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
