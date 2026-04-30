// header files

#include <GLFW/glfw3.h>
#include <string>

// ImGui backends for GLFW and DirectX 11
#include "../external/imgui/backends/imgui_impl_dx11.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"

// Engine headers
#include "Engine.h"
#include "Engine/core/render/d3d11/DXRender.h"
#include "Engine/graphics/Window.h"
#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Editor.h"

class Application {
public:
    Application() : m_renderer(), m_initialized(false) {}

    bool Initialize() {
        bool CreatedWindow = CreateEditorWindow("Deform Editor", WindowGraphicsAPI::DX11);
        bool RendererInitialized = CreateRenderer();
        bool ImGuiInitialized = InitializeImGui();

        if (!CreatedWindow || !RendererInitialized || !ImGuiInitialized) {
            deform::Logger::FatalError("Application initialization failed.");
            return false;
        }

        glfwSetWindowUserPointer(m_window.GetWindow(), this);
        glfwSetFramebufferSizeCallback(m_window.GetWindow(), FramebufferSizeCallback);
        glfwMaximizeWindow(m_window.GetWindow());

        ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(m_window.GetWidth()),
                                            static_cast<float>(m_window.GetHeight()));

        m_initialized = true;

        // Crea scena di default
        m_state.scene->CreateNewScene();

        return true;
    }

    int Run() {
        deform::Logger::Log("Entering main application loop.");
        deform::Logger::Log("ImGui Display Size: " + std::to_string(ImGui::GetIO().DisplaySize.x) +
                           "x" + std::to_string(ImGui::GetIO().DisplaySize.y));

        while (!m_window.ShouldClose()) {
            m_window.PollEvents();
            m_renderer.BeginFrame(m_state.clearColor);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            UpdateDockHost();
            UpdatePanels();

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            m_renderer.Present();
        }

        return 0;
    }

    void Shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        if (m_renderer.IsInitialized())
            m_renderer.Shutdown();

        m_window.Destroy();
        glfwTerminate();
    }

private:
    /// Called when the framebuffer size is resized, updating the renderer's viewport and logging the change.
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->OnFramebufferResize(width, height);
        }
    }

    /// Called when the framebuffer size is resized, updating the renderer's viewport and logging the change.
    void OnFramebufferResize(int width, int height) {
        deform::Logger::Log("Display size updated: " + std::to_string(width) + "x" +
                            std::to_string(height));
        deform::Logger::Log("Framebuffer size callback triggered: " +
                            std::to_string(width) + "x" + std::to_string(height));
        m_renderer.Resize(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    }

    /// Updates the panels in the editor, including the main bar, hierarchy, inspector, and console.
    void UpdatePanels() {
        Editor::UI::MainBar::UpdateMainBar(m_state);

        if (m_state.showHierarchy)
            Editor::UI::Hierarchy::UpdateHierarchy(m_state);
        if (m_state.showInspector)
            Editor::UI::Inspector::UpdateInspector(m_state);
        if (m_state.showConsole)
            Editor::UI::Console::UpdateConsole();
        if (m_state.showScene) {
            ImGui::Begin("Scene", &m_state.showScene);
            ImVec2 size = ImGui::GetContentRegionAvail();
            if (m_renderer.m_sceneSRV)
                ImGui::Image((ImTextureID)m_renderer.m_sceneSRV, size);
            else
                ImGui::TextUnformatted("(Scene texture not available)");
            ImGui::End();
        }
    }

    /// Updates the dock host window, setting its position and size to cover the main viewport.
    void UpdateDockHost() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGuiWindowFlags hostWindowFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("DockSpace Host", nullptr, hostWindowFlags);
        ImGui::PopStyleVar(2);
        ImGui::DockSpace(ImGui::GetID("DockSpace Host"),
                         ImVec2(m_window.GetWidth(), m_window.GetHeight()),
                         ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }

    /// Initializes the renderer with the window's graphics context.
    /// Returns true if the renderer was successfully initialized, false otherwise.
    bool CreateRenderer() {
        bool d3d11 = m_renderer.Initialize(m_window.GetWindow());
        if (!d3d11) {
            deform::Logger::FatalError("Renderer initialization failed.");
            Shutdown();
            return false;
        }
        return d3d11;
    }

    /// Creates an editor window with the specified title and graphics API.
    /// Returns true if the window was successfully created, false otherwise.
    bool CreateEditorWindow(const char* WindowTitle, WindowGraphicsAPI api = WindowGraphicsAPI::DX11) {
        bool CreatedWindow = m_window.Create(WindowTitle, "Window_Icon.png", 1280, 720, api);
        if (!CreatedWindow) {
            deform::Logger::FatalError("Window failed");
            return false;
        }
        deform::Logger::Log("Window " + std::string(WindowTitle) + " created successfully.");
        return CreatedWindow;
    }

    bool InitializeImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

        bool ImGuiGlfwInitialized = ImGui_ImplGlfw_InitForOther(m_window.GetWindow(), true);
        bool ImguiDX11Initialized = ImGui_ImplDX11_Init(m_renderer.GetDevice(), m_renderer.GetContext());
        if (!ImGuiGlfwInitialized) {
            deform::Logger::FatalError("ImGui GLFW backend initialization failed.");
            Shutdown();
            return false;
        }
        if (!ImguiDX11Initialized) {
            deform::Logger::FatalError("ImGui DX11 backend initialization failed.");
            Shutdown();
            return false;
        }
        deform::Logger::Log("ImGui GLFW backend initialized successfully.");
        deform::Logger::Log("ImGui DX11 backend initialized successfully.");
        return true;
    }

    Window m_window;
    DXRender m_renderer;
    Editor::UIState m_state;
    bool m_initialized;
};

int main() {
    Application app;
    if (!app.Initialize()) {
        return -1;
    }
    int result = app.Run();
    app.Shutdown();
    return result;
}
