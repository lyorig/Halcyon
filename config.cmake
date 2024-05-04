# Halcyon CMake configuration. include() this and you should be good to go.

# Make sure SDL is present on the system.
find_package(SDL2       REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf   REQUIRED CONFIG)

# Include directores.
set(HALCYON_INCLUDE_DIRS ${LYOSTL_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/include/)

file(GLOB_RECURSE HALCYON_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Libraries.
set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)