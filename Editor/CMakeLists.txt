set(EDITOR_SOURCES
    src/main.cpp
    src/Editor.h
    src/EditorUIComponents/Assets.cpp
    src/EditorUIComponents/Hierarchy.cpp
    src/EditorUIComponents/Inspector.cpp
    src/EditorUIComponents/MenuBar.cpp
)

set(IMGUI_COMMON_SOURCES
    "external/imgui/imgui.cpp"
    "external/imgui/imgui_draw.cpp"
    "external/imgui/imgui_tables.cpp"
    "external/imgui/imgui_widgets.cpp"
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

if(WIN32)
    set(IMGUI_PLATFORM_SOURCES
        "external/imgui/backends/imgui_impl_dx11.cpp"
    )
else()
    set(IMGUI_PLATFORM_SOURCES
        "external/imgui/backends/imgui_impl_opengl3.cpp"
    )
endif()

add_executable(Editor ${EDITOR_SOURCES} ${IMGUI_COMMON_SOURCES} ${IMGUI_PLATFORM_SOURCES})

target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/assets
    src
    external/imgui
    external/imgui/backends
    external/UUID/include
    external
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Physics/src
    ${CMAKE_SOURCE_DIR}/Scripting/src
)

if(WIN32)
    target_include_directories(Editor PRIVATE external/GLFW/include)
    target_link_libraries(Editor PRIVATE
        Engine
        "${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/lib-vc2022/glfw3.lib"
        d3d11
        dxgi
    )
else()
    # Linux — usa GLFW e OpenGL dal sistema
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)

    target_link_libraries(Editor PRIVATE
        Engine
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS}
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Runtime)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Archive)

if(WIN32)
    add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Engine>" "$<TARGET_FILE_DIR:Editor>/Engine.dll"
        COMMENT "Copying Engine.dll to Editor directory"
    )
endif()

set_property(TARGET Editor PROPERTY CXX_STANDARD 23)