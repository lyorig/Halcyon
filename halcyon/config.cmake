cmake_minimum_required(VERSION 3.12)

include_directories(${CMAKE_CURRENT_LIST_DIR})

list(APPEND HALCYON_SOURCES
src/debug.cpp
src/engine.cpp
src/image_loader.cpp
src/input_handler.cpp
src/mixer.cpp
src/renderer.cpp
src/texture.cpp
src/ttf_engine.cpp
src/window.cpp
src/components/chunk.cpp
src/components/font.cpp
src/components/music.cpp
src/components/surface.cpp
src/internal/subsystem.cpp
src/internal/printing.cpp)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/halcyon/)

list(APPEND HALCYON_LIBRARIES
SDL2
SDL2main
SDL2_image
SDL2_ttf
SDL2_mixer)