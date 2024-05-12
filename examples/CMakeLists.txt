cmake_minimum_required(VERSION 3.22.0)

project("Halcyon Examples" VERSION 1.0)

include(../config.cmake)

include_directories(${HALCYON_INCLUDE_DIRS})
link_libraries(${HALCYON_LIBRARIES})

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(HAL_DEBUG_ENABLED)
endif()

add_compile_definitions(HAL_NO_EXIT_ON_PANIC)

add_library(Halcyon OBJECT ${HALCYON_SOURCES})

file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS *.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

foreach(FILE IN LISTS EXAMPLE_SOURCES)
    get_filename_component(TGT_NAME ${FILE} NAME_WE)
    add_executable(${TGT_NAME} ${FILE} $<TARGET_OBJECTS:Halcyon>)
endforeach()