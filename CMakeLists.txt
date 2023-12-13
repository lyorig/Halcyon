cmake_minimum_required(VERSION 3.22.1)

if(MSVC)
  add_compile_options(/Wall)
else()
  add_compile_options(-Wall)
endif()

if (APPLE)
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
endif()

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

project(Halcyon VERSION 0.1 LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_executable(Halcyon main.cpp mono_app.cpp halcyon/engine.cpp halcyon/image_loader.cpp halcyon/ttf_engine.cpp halcyon/mixer.cpp halcyon/input_handler.cpp halcyon/debug.cpp halcyon/console.cpp halcyon/texture.cpp halcyon/spritesheet.cpp halcyon/components/music.cpp halcyon/components/chunk.cpp halcyon/window.cpp halcyon/components/surface.cpp halcyon/components/font.cpp halcyon/components/renderer.cpp)

set_property(TARGET Halcyon PROPERTY CXX_STANDARD 23)

target_include_directories(Halcyon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Halcyon SDL2 SDL2_image SDL2_ttf SDL2_mixer)