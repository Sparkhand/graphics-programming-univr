# CMakeLists.txt for LabOpenGL

## Initial setup
## ----------------------------

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.21)

# Project's name
project(LabOpenGL)

# A list of the exercises
list(APPEND EXERCISES
    "1.1.1.hello_window"
)

# Short names for directories
set(PROJECT_VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

## Compiler settings and additional libraries
## ----------------------------

# Build GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_VENDOR_DIR}/glfw)

# Build ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_VENDOR_DIR}/assimp)

# Add useful compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

## Target
## ----------------------------

# Create targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

foreach(EXERCISE ${EXERCISES})
    file(GLOB EXERCISE_SOURCES
        "src/${EXERCISE}/*.h"
        "src/${EXERCISE}/*.hpp"
        "src/${EXERCISE}/*.cpp"
        "src/${EXERCISE}/*.vs"
        "src/${EXERCISE}/*.fs"
        "src/${EXERCISE}/*.tcs"
        "src/${EXERCISE}/*.tes"
        "src/${EXERCISE}/*.gs"
        "src/${EXERCISE}/*.cs"
    )

    add_executable(${EXERCISE}
        ${EXERCISE_SOURCES}
        "vendor/glad/src/glad.c"
    )

    target_include_directories(${EXERCISE}
        PRIVATE "include/"
        PRIVATE "vendor/assimp/include/"
        PRIVATE "vendor/glad/include/"
        PRIVATE "vendor/glfw/include/"
        PRIVATE "vendor/glm/"
        PRIVATE "vendor/stb/"
    )

    target_compile_definitions(${EXERCISE}
        PRIVATE "GLFW_INCLUDE_NONE"
        PRIVATE "PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
    )

    target_link_libraries(${EXERCISE}
        glfw
        assimp
        ${GLFW_LIBRARIES}
        ${GLAD_LIBRARIES}
    )
    
endforeach(EXERCISE ${EXERCISES})
