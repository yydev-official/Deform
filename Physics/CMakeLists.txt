cmake_minimum_required(VERSION 3.22)
project(DeformPhysics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PHYSICS_SOURCES
    src/PhysicsAPI.h
    src/Collider.h
    src/Collider.cpp
    src/Rigidbody.h
    src/Rigidbody.cpp
    src/PhysicsComponent.h
    src/PhysicsWorld.h
    src/PhysicsWorld.cpp
)

add_library(DeformPhysics SHARED ${PHYSICS_SOURCES})

target_compile_definitions(DeformPhysics PRIVATE PHYSICS_EXPORTS)

target_include_directories(DeformPhysics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Engine/src    # accesso a ECS, Vector, ecc.
)

target_link_libraries(DeformPhysics PUBLIC DeformEngine)

# Copia la DLL nella cartella dell'editor dopo la build
add_custom_command(TARGET DeformPhysics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:DeformPhysics>
        ${CMAKE_SOURCE_DIR}/Editor/bin/$<CONFIG>/DeformPhysics.dll
)
