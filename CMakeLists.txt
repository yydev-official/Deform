cmake_minimum_required(VERSION 3.10)

project(TrailEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------
# GLAD
# --------------------
add_library(glad
    src/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/src/external/glad/include
)

# --------------------
# GLFW (build from source)
# --------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(src/external/GLFW)

# --------------------
# OpenGL
# --------------------
find_package(OpenGL REQUIRED)

# --------------------
# App
# --------------------
add_executable(TrailEngine
    src/main.cpp
    src/graphics/Window.cpp
)

target_include_directories(TrailEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(TrailEngine PRIVATE
    glfw
    glad
    OpenGL::GL
)

# --------------------
# Windows Settings
# --------------------
if (WIN32)
    set_target_properties(TrailEngine PROPERTIES
        WIN32_EXECUTABLE FALSE
    )

    target_link_libraries(TrailEngine PRIVATE
        d3d11
        dxgi
        d3dcompiler
    )
endif()
