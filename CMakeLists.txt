cmake_minimum_required(VERSION 3.22)

project(SpaceInvadersClone LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES src/**.cpp)
add_executable(sic ${SOURCES} ext/SDL_FontCache.c)

find_package(SDL2 REQUIRED)
#find_package(SDL2_ttf REQUIRED)
#find_package(SDL2_image REQUIRED)

include_directories(src/ ext/ ${SDL2_INCLUDE_DIRS})
target_link_libraries(sic ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)
