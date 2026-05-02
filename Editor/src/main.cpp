// header files

#include <GLFW/glfw3.h>
#include <string>
#include <filesystem>

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

        while (!m_window.ShouldClose()) {
            m_window.PollEvents();

            // ── 1. Render scene into off-screen texture ──────────────
            RenderScene();

            // ── 2. Begin backbuffer pass for ImGui ──────────────────
            m_renderer.BeginBackbufferPass(m_state.clearColor);

            // ── 3. ImGui frame ───────────────────────────────────────
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            UpdateDockHost();
            UpdatePanels();

            ImGui::Render();

            // Restore backbuffer before ImGui draw (ImGui may rebind RTs)
            m_renderer.BeginBackbufferPass(m_state.clearColor);
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

    /// Renders the 3D scene into the off-screen scene texture
    void RenderScene() {
        auto* ctx = m_renderer.GetContext();
        auto* sceneRTV = m_renderer.GetSceneRenderTargetView();
        if (!ctx || !sceneRTV) return;

        // Clear scene texture with a dark background
        const float sceneClear[4] = { 0.12f, 0.12f, 0.12f, 1.0f };
        ctx->OMSetRenderTargets(1, &sceneRTV, nullptr);
        ctx->ClearRenderTargetView(sceneRTV, sceneClear);

        // TODO: actual draw calls go here once mesh rendering is implemented
        // For now the scene panel shows a cleared color (proves the pipeline works)
    }

    /// Applies a dark editor theme to ImGui
    void ApplyEditorTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_WindowBg]          = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
        colors[ImGuiCol_ChildBg]           = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
        colors[ImGuiCol_PopupBg]           = ImVec4(0.15f, 0.15f, 0.16f, 1.00f);
        colors[ImGuiCol_Border]            = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBg]           = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]    = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive]     = ImVec4(0.26f, 0.26f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg]           = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive]     = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_MenuBarBg]         = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]       = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]     = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
        colors[ImGuiCol_CheckMark]         = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab]        = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]            = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
        colors[ImGuiCol_ButtonHovered]     = ImVec4(0.30f, 0.50f, 0.80f, 1.00f);
        colors[ImGuiCol_ButtonActive]      = ImVec4(0.25f, 0.45f, 0.75f, 1.00f);
        colors[ImGuiCol_Header]            = ImVec4(0.20f, 0.35f, 0.60f, 1.00f);
        colors[ImGuiCol_HeaderHovered]     = ImVec4(0.30f, 0.50f, 0.80f, 1.00f);
        colors[ImGuiCol_HeaderActive]      = ImVec4(0.25f, 0.45f, 0.75f, 1.00f);
        colors[ImGuiCol_Tab]               = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
        colors[ImGuiCol_TabHovered]        = ImVec4(0.30f, 0.50f, 0.80f, 1.00f);
        colors[ImGuiCol_TabActive]         = ImVec4(0.20f, 0.35f, 0.60f, 1.00f);
        colors[ImGuiCol_TabUnfocused]      = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]= ImVec4(0.15f, 0.25f, 0.45f, 1.00f);
        colors[ImGuiCol_DockingPreview]    = ImVec4(0.40f, 0.70f, 1.00f, 0.70f);
        colors[ImGuiCol_Text]              = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled]      = ImVec4(0.45f, 0.45f, 0.50f, 1.00f);
        colors[ImGuiCol_Separator]         = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);

        style.WindowRounding    = 4.0f;
        style.FrameRounding     = 3.0f;
        style.GrabRounding      = 3.0f;
        style.TabRounding       = 3.0f;
        style.FramePadding      = ImVec2(6, 4);
        style.ItemSpacing       = ImVec2(8, 5);
        style.WindowPadding     = ImVec2(8, 8);
        style.IndentSpacing     = 16.0f;
        style.ScrollbarSize     = 12.0f;
    }

    /// Updates the panels in the editor
    void UpdatePanels() {
        Editor::UI::MainBar::UpdateMainBar(m_state);

        if (m_state.showHierarchy)
            Editor::UI::Hierarchy::UpdateHierarchy(m_state);
        if (m_state.showInspector)
            Editor::UI::Inspector::UpdateInspector(m_state);
        if (m_state.showConsole)
            Editor::UI::Console::UpdateConsole();
        if (m_state.showAssets)
            Editor::UI::Assets::UpdateAssets(m_state);
        if (m_state.showScene) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Scene", &m_state.showScene);
            ImGui::PopStyleVar();

            ImVec2 size = ImGui::GetContentRegionAvail();

            // Unbind scene SRV before using as RTV (prevents DX11 warning)
            ID3D11ShaderResourceView* nullSRV = nullptr;
            m_renderer.GetContext()->PSSetShaderResources(0, 1, &nullSRV);

            if (m_renderer.GetSceneTextureView() && size.x > 0 && size.y > 0)
                ImGui::Image((ImTextureID)m_renderer.GetSceneTextureView(), size);
            else
                ImGui::TextUnformatted("Scene texture not available.");

            ImGui::End();
        }
    }

    /// Updates the dock host window
    void UpdateDockHost() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags hostFlags =
            ImGuiWindowFlags_NoTitleBar      | ImGuiWindowFlags_NoCollapse  |
            ImGuiWindowFlags_NoResize        | ImGuiWindowFlags_NoMove      |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,  0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("##DockHost", nullptr, hostFlags);
        ImGui::PopStyleVar(3);

        ImGui::DockSpace(ImGui::GetID("MainDockSpace"), ImVec2(0, 0),
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
        io.ConfigFlags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

        // Load font from assets if available, otherwise use default
        const char* fontPath = "assets/Fonts/font.ttf";
        if (std::filesystem::exists(fontPath))
            io.Fonts->AddFontFromFileTTF(fontPath, 15.0f);
        else
            io.Fonts->AddFontDefault();

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

        ApplyEditorTheme();

        deform::Logger::Log("ImGui initialized successfully.");
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
