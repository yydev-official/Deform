cmake_minimum_required(VERSION 3.22)
project(DeformScripting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Lua (trovalo con find_package o usa il percorso diretto) ──
find_package(Lua REQUIRED)

set(SCRIPTING_SOURCES
    src/ScriptingAPI.h
    src/LuaState.h
    src/LuaState.cpp
)

add_library(DeformScripting SHARED ${SCRIPTING_SOURCES})

target_compile_definitions(DeformScripting PRIVATE SCRIPTING_EXPORTS)

target_include_directories(DeformScripting PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Physics/src
    ${LUA_INCLUDE_DIR}
)

target_link_libraries(DeformScripting PUBLIC
    DeformEngine
    DeformPhysics
    ${LUA_LIBRARIES}
)

# Copia la DLL nella cartella dell'editor dopo la build
add_custom_command(TARGET DeformScripting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:DeformScripting>
        ${CMAKE_SOURCE_DIR}/Editor/bin/$<CONFIG>/DeformScripting.dll
)
