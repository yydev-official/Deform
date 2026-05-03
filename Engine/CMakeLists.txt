if(WIN32)
    set(ENGINE_PLATFORM_SOURCES
        src/Engine/core/render/d3d11/DXRender.cpp
        src/Engine/core/render/d3d11/DXRender.h
    )
else()
    set(ENGINE_PLATFORM_SOURCES "")
endif()

set(ENGINE_SOURCES
    src/Engine.h
    src/Engine/EngineAPI.h
    src/Engine/core/logger.cpp
    src/Engine/core/maths/vector/Vector.cpp
    src/Engine/core/maths/vector/Vector.h
    src/Engine/core/maths/Matrix4x4.cpp
    src/Engine/core/maths/Matrix4x4.h
    src/Engine/core/maths/Quaternion.cpp
    src/Engine/core/maths/Quaternion.h
    src/Engine/core/Object/Object.cpp
    src/Engine/core/Object/Object.h
    src/Engine/core/ecs/ECS.cpp
    src/Engine/core/ecs/ECS.h
    src/Engine/core/render/IRenderer.h
    src/Engine/core/render/opengl/GLRender.cpp
    src/Engine/core/render/opengl/GLRender.h
    src/Engine/core/render/vulkan/VKRender.cpp
    src/Engine/core/render/vulkan/VKRender.h
    src/Engine/graphics/Window.cpp
    src/Engine/graphics/Window.h
    ${ENGINE_PLATFORM_SOURCES}
)

find_package(Vulkan)
add_library(Engine SHARED ${ENGINE_SOURCES})

target_compile_definitions(Engine PRIVATE DEFORM_ENGINE_EXPORTS)

if(WIN32)
    target_include_directories(Engine PUBLIC
        src
        external/GLFW/include
        external/UUID/include
        external/DDSTextureLoader
        external
    )
    target_link_libraries(Engine PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/lib-vc2022/glfw3.lib"
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
        opengl32.lib
    )
else()
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)

    target_include_directories(Engine PUBLIC
        src
        external/UUID/include
        external
    )
    target_link_libraries(Engine PRIVATE
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS}
    )
endif()

if(Vulkan_FOUND)
    target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(Engine PRIVATE Vulkan::Vulkan)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Runtime)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Archive)
