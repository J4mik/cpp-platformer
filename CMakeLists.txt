cmake_minimum_required(VERSION 3.15...3.31)
project(Defblade VERSION 1.0
                  DESCRIPTION "Pixel art platformer"
                  LANGUAGES CXX)

set(CMAKE_CPP_COMPILER g++)
set(CMAKE_MAKE_PROGRAM ninja)

# for release
# set(CMAKE_CXX_FLAGS -mwindows)

# sources
set(SOURCES main.cpp)# src/tiles.cpp)

# -Iinclude
include_directories(${CMAKE_SOURCE_DIR}/include)
# -Llib
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(Defblade ${SOURCES})

# libraries to compile with -lSDL2main ...
set(SDL2_LIBRARIES mingw32 SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)

# link it so ld can find it
target_link_libraries(Defblade PUBLIC ${SDL2_LIBRARIES})

# target_sources(Defblade PRIVATE resources.rc)