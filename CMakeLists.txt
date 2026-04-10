cmake_minimum_required(VERSION 3.10)
project(SFML_Linux)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CSS_STANDARD_REQUIRED ON)

include(FetchContent)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
)

FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS src/*.cpp src/*.c)

add_executable(SFML_Linux ${SRCS})

target_link_libraries(SFML_Linux
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

add_custom_command(TARGET SFML_Linux POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:SFML_Linux>"
        "${CMAKE_SOURCE_DIR}/src/SFML_Linux${CMAKE_EXECUTABLE_SUFFIX}"
    VERBATIM
)