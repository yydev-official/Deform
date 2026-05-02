cmake_minimum_required(VERSION 3.22)
project(Deform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(Physics)
add_subdirectory(Scripting)
add_subdirectory(Editor)