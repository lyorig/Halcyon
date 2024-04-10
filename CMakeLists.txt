cmake_minimum_required(VERSION 3.22.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

include(config.cmake)

# Run tests.
include_directories(${HALCYON_INCLUDE_DIRS})
link_libraries(${HALCYON_LIBRARIES})

add_executable(HalTest test/main.cpp ${HALCYON_SOURCES})

enable_testing()

add_test(NAME WindowResize COMMAND $<TARGET_FILE:HalTest> --window-resize)
add_test(NAME BasicInit COMMAND $<TARGET_FILE:HalTest> --basic-init)