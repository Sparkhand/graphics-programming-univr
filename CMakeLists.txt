# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.21)

# Project's name
project(LabOpenGL)

# Create target
add_executable("prova"
    "src/main.cpp"
    "vendor/glad/src/glad.c"
)

# Short names for directories
set(PROJECT_VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

# Build GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_VENDOR_DIR}/glfw)

# Build ASSIMP
# option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
# option(ASSIMP_BUILD_SAMPLES OFF)
# option(ASSIMP_BUILD_TESTS OFF)
# add_subdirectory(${PROJECT_VENDOR_DIR}/assimp)

# Add useful compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

target_include_directories("prova"
    PRIVATE "vendor/glad/include"
    PRIVATE "vendor/glfw/include"
)


target_compile_definitions("prova"
    PRIVATE "GLFW_INCLUDE_NONE"
    PRIVATE "PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
)

target_link_libraries("prova"
    glfw
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
)