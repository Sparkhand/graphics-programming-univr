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
    "1.3.3.shaders_class"
    "1.4.1.textures"
    "1.4.2.textures_combined"
)

# Short names for directories
set(PROJECT_VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(PROJECT_INCLUDES_DIR ${PROJECT_SOURCE_DIR}/include)

# Set CMake to run configure every time a file changes in:
# src, resources, include or vendor
file(GLOB_RECURSE TO_TRACK
    "${PROJECT_SOURCE_DIR}/src/*"
    "${PROJECT_SOURCE_DIR}/resources/*"
    "${PROJECT_SOURCE_DIR}/include/*"
    "${PROJECT_SOURCE_DIR}/vendor/*"    
)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${TO_TRACK}
)

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

# Project include directories
include_directories(
    "${PROJECT_INCLUDES_DIR}"
    "${PROJECT_VENDOR_DIR}/assimp/include/"
    "${PROJECT_VENDOR_DIR}/glad/include/"
    "${PROJECT_VENDOR_DIR}/glfw/include/"
    "${PROJECT_VENDOR_DIR}/glm/"
    "${PROJECT_VENDOR_DIR}/stb/"
)

# Link libraries
link_libraries(
    assimp
    glfw
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
)

# Compiler definitions
add_definitions(
    -DGLFW_INCLUDE_NONE
    -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

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
        "src/${EXERCISE}/*.jpg"
        "src/${EXERCISE}/*.png"
    )

    add_executable(${EXERCISE}
        ${EXERCISE_SOURCES}
        ${PROJECT_VENDOR_DIR}/glad/src/glad.c
    )
    
    # Filter EXERCISE_SOURCES for textures and shaders and copy them to the build directory
    file(GLOB RESOURCES
        "src/${EXERCISE}/*.vs"
        "src/${EXERCISE}/*.fs"
        "src/${EXERCISE}/*.tcs"
        "src/${EXERCISE}/*.tes"
        "src/${EXERCISE}/*.gs"
        "src/${EXERCISE}/*.cs"
        "src/${EXERCISE}/*.jpg"
        "src/${EXERCISE}/*.png"
    )
    foreach(RESOURCE ${RESOURCES})
        file(COPY ${RESOURCE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach(RESOURCE ${RESOURCES})
endforeach(EXERCISE ${EXERCISES})


## Resources
## ----------------------------


# Copy directory "resources" to the build directory
file(COPY "resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})