include(halcyon/config.cmake)

set(QUEST_SOURCES
ents/components.cpp
ents/entities.cpp
camera.cpp
game.cpp
main.cpp
spritesheet.cpp
)

list(TRANSFORM QUEST_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

set(QUEST_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin/)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)