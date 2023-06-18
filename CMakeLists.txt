cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wpedantic)
endif()

project(HalodaQuest VERSION 0.1 LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_executable(HalodaQuest main.cpp halcyon/engine.cpp halcyon/image_loader.cpp halcyon/font_loader.cpp halcyon/mixer.cpp halcyon/input_handler.cpp halcyon/debug.cpp halcyon/console.cpp halcyon/texture.cpp halcyon/spritesheet.cpp halcyon/components/music.cpp halcyon/components/chunk.cpp halcyon/window.cpp halcyon/components/surface.cpp halcyon/components/font.cpp halcyon/components/renderer.cpp intro.cpp)

target_include_directories(HalodaQuest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HalodaQuest SDL2 SDL2_image SDL2_ttf SDL2_mixer)