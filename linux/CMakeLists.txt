cmake_minimum_required(VERSION 3.4.1)

project(timetravel)

set(MAIN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

include_directories(/usr/local/include/stb)
include_directories(/usr/include/freetype2)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

add_executable(
    timetravel ${EXECUTABLE_TYPE}
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

set_property(TARGET timetravel PROPERTY CXX_STANDARD 17)

target_link_libraries(
    timetravel
    libGL.so
    libglfw.so
    libGLEW.so
    libfreetype.so
)

file(GLOB_RECURSE SHADER_SOURCES
    ${MAIN_SOURCE_DIR}/shaders/*.frag
    ${MAIN_SOURCE_DIR}/shaders/*.vert
)

add_custom_target(
    shaders
    DEPENDS ${SPIRV_BINARY_FILES}
    SOURCES ${SHADER_SOURCES}
)

add_dependencies(timetravel shaders)

add_custom_command(TARGET timetravel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:timetravel>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${MAIN_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:timetravel>/shaders"
)
