set(EDITOR_SOURCES
    src/main.cpp
    src/Editor.h
    src/EditorUIComponents/Console.cpp
    src/EditorUIComponents/Hierarchy.cpp
    src/EditorUIComponents/Inspector.cpp
    src/EditorUIComponents/MenuBar.cpp
)

set(IMGUI_SOURCES
    "external/imgui/imgui.cpp"
    "external/imgui/imgui_draw.cpp"
    "external/imgui/imgui_tables.cpp"
    "external/imgui/imgui_widgets.cpp"
    "external/imgui/backends/imgui_impl_dx11.cpp"
    "external/imgui/backends/imgui_impl_glfw.cpp"
)

add_executable(Editor ${EDITOR_SOURCES} ${IMGUI_SOURCES})

target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/assets
    src
    external/imgui
    external/imgui/backends
    external/UUID/include
    external/GLFW/include
    external
    ${CMAKE_SOURCE_DIR}/Engine/src
)

target_link_libraries(Editor PRIVATE 
    Engine
    "${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/lib-vc2022/glfw3.lib"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Runtime)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Editor/$<CONFIG>/Archive)

# Copy Engine.dll to Editor folder after build
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Engine>" "$<TARGET_FILE_DIR:Editor>/Engine.dll"
    COMMENT "Copying Engine.dll to Editor directory"
)

set_property(TARGET Editor PROPERTY CXX_STANDARD 23)