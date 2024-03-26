set(ALPP_SOURCES
debug.cpp
buffer.cpp
decoder.cpp
listener.cpp
source.cpp
state.cpp
)

list(TRANSFORM ALPP_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

# Config mode ensures Apple's OpenAL implementation
# isn't chosen on macOS.
find_package(OpenAL REQUIRED CONFIG)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
link_libraries(OpenAL::OpenAL)