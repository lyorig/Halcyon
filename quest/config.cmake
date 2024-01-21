include_directories(${CMAKE_CURRENT_LIST_DIR})

set(QUEST_SOURCES
main.cpp
game.cpp
sprite.cpp
states/base.cpp
states/intro.cpp
states/menu.cpp
states/playing.cpp
states/paused.cpp)

list(TRANSFORM QUEST_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/quest/src/)