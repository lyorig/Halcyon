include(halcyon/config.cmake)

set(ASSET2CODE_SOURCES
main.cpp
)

list(TRANSFORM ASSET2CODE_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

set(ASSET2CODE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin/)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)