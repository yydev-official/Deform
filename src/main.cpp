// main.cpp
#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <cstdio>

#include "init.h"
#include "Editor/Editor.h"

Init Initializer;
DXRender d3d11renderer;
Window GLFWHandler;


// Editor stuff
using namespace Editor;

UI editorUI;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    if (!Initializer.InitGLFW()) { Logger::Log("InitGLFW FAILED\n");  return -1; }
    if (!Initializer.InitD3d11()) { Logger::Log("InitD3d11 FAILED\n"); return -1; }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Logger::Log("Initialization complete. Entering main loop.");



    while (!glfwWindowShouldClose(GLFWHandler.GetWindow()))
    {
        glfwPollEvents();

        
        if (d3d11renderer.m_sceneRTV)
        {
            float sceneClear[4] = { 0.15f, 0.15f, 0.5f, 1.0f };
            d3d11renderer.m_context->OMSetRenderTargets(1, &d3d11renderer.m_sceneRTV, nullptr);
            d3d11renderer.m_context->ClearRenderTargetView(d3d11renderer.m_sceneRTV, sceneClear);
        }

        ID3D11DepthStencilView* nullDSV = nullptr;

        float clearColor[4] = { 0.05f, 0.5f, 1.0f, 0.0f };
        d3d11renderer.m_context->OMSetRenderTargets(1, &d3d11renderer.m_rtv, nullptr);
        d3d11renderer.m_context->ClearRenderTargetView(d3d11renderer.m_rtv, clearColor);


        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("DockSpaceHost", nullptr, window_flags);

        ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Always);
        ImGui::Text("%.1f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

        ImGui::End();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New Scene");
                ImGui::MenuItem("Open...");
                ImGui::MenuItem("Save");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                static bool showHierarchy = true;
                static bool showInspector = true;
                static bool showScene = true;
                static bool showConsole = true;
                ImGui::MenuItem("Hierarchy", nullptr, &showHierarchy);
                ImGui::MenuItem("Inspector", nullptr, &showInspector);
                ImGui::MenuItem("Scene", nullptr, &showScene);
                ImGui::MenuItem("Console", nullptr, &showConsole);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_viewport->ID);

        static bool showHierarchy = true;
        static bool showInspector = true;
        static bool showScene = true;
        static bool showConsole = true;

        if (showHierarchy)
        {
            ImGui::Begin("Hierarchy", &showHierarchy);
            ImGui::TextUnformatted("Scene");
            ImGui::Separator();
            ImGui::Selectable("Camera");
            ImGui::Selectable("Light");
            ImGui::Selectable("Cube");
            ImGui::End();
        }

        if (showInspector)
        {
            ImGui::Begin("Inspector", &showInspector);
            ImGui::TextUnformatted("Select an object in the Hierarchy");
            ImGui::Separator();
            ImGui::TextUnformatted("Transform");

            static float pos[3] = { 0.0f, 0.0f, 0.0f };
            static float rot[3] = { 0.0f, 0.0f, 0.0f };
            static float scl[3] = { 1.0f, 1.0f, 1.0f };

            ImGui::DragFloat3("Position", pos, 0.1f);
            ImGui::DragFloat3("Rotation", rot, 0.1f);
            ImGui::DragFloat3("Scale", scl, 0.1f);

            ImGui::End();
        }

        if (showScene)
        {
            ImGui::Begin("Scene", &showScene);
            ImVec2 size = ImGui::GetContentRegionAvail();
            if (d3d11renderer.m_sceneSRV)
                ImGui::Image((ImTextureID)d3d11renderer.m_sceneSRV, size);
            else
                ImGui::TextUnformatted("(Scene texture not available)");
            ImGui::End();
        }

        if (showConsole)
        {
            ImGui::Begin("Console", &showConsole);

            const auto& logs = Logger::GetLogs();

            for (const auto& log : logs)
            {
                ImGui::Text("%s", log.c_str());
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        d3d11renderer.m_swapChain->Present(1, 0);
    }


	Logger::Log("Exiting main loop. Cleaning up and shutting down.");
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    d3d11renderer.Resize((UINT)width, (UINT)height);
}

bool Init::InitGLFW()
{
    if (!glfwInit())
        return false;

    if (!GLFWHandler.Create(1280, 720))
    {
        glfwTerminate();
        return false;
    }

    if (!GLFWHandler.GetWindow())
    {
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(GLFWHandler.GetWindow(), framebuffer_size_callback);

	Logger::Log("GLFW initialization successful.");
    return true;
}

bool Init::InitD3d11()
{
    if (!d3d11renderer.Initialize(GLFWHandler.GetWindow()))
        return false;
    if (!d3d11renderer.InitializeImgui(GLFWHandler.GetWindow()))
        return false;
    return true;
}