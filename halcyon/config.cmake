cmake_minimum_required(VERSION 3.12)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(HALCYON_SOURCES
debug.cpp
engine.cpp
image_loader.cpp
input_handler.cpp
mixer.cpp
renderer.cpp
texture.cpp
ttf_engine.cpp
window.cpp
components/chunk.cpp
components/font.cpp
components/music.cpp
components/surface.cpp
internal/subsystem.cpp
internal/printing.cpp)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/halcyon/src/)

set(HALCYON_LIBRARIES
SDL2
SDL2main
SDL2_image
SDL2_ttf
SDL2_mixer)