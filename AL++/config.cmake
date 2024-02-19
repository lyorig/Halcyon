set(ALPP_SOURCES
debug.cpp
buffer.cpp
decoder.cpp
listener.cpp
source.cpp
state.cpp
)

list(TRANSFORM ALPP_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

find_package(OpenAL REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
link_libraries(OpenAL::OpenAL)