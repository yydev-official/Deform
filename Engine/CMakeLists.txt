set(ENGINE_SOURCES
    src/Engine.h
    src/Engine/EngineAPI.h
    src/Engine/core/logger.cpp
    src/Engine/core/maths/vector/Vector.cpp
    src/Engine/core/maths/vector/Vector.h
    src/Engine/core/Object/Object.cpp
    src/Engine/core/Object/Object.h
    src/Engine/core/render/IRenderer.h
    src/Engine/core/render/d3d11/DXRender.cpp
    src/Engine/core/render/d3d11/DXRender.h
    src/Engine/core/render/opengl/GLRender.cpp
    src/Engine/core/render/opengl/GLRender.h
    src/Engine/core/render/vulkan/VKRender.cpp
    src/Engine/core/render/vulkan/VKRender.h
    src/Engine/core/scene/scene.cpp
    src/Engine/core/scene/scene.h
    src/Engine/graphics/Window.cpp
    src/Engine/graphics/Window.h
)

find_package(Vulkan)  # Optional - only needed if using VKRender
add_library(Engine SHARED ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC
    src
    external/GLFW/include
    external/UUID/include
    external/DDSTextureLoader
    external
)

if(Vulkan_FOUND)
        target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIRS})
endif()

target_link_libraries(Engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/lib-vc2022/glfw3.lib"
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
    opengl32.lib
)

if(Vulkan_FOUND)
        target_link_libraries(Engine PRIVATE Vulkan::Vulkan)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Runtime)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Engine/$<CONFIG>/Archive)

target_compile_definitions(Engine PRIVATE DEFORM_ENGINE_EXPORTS)
