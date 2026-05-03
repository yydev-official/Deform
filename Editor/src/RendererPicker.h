#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"

#ifdef _WIN32
    #include "imgui_impl_dx11.h"
    #include <d3d11.h>
    #include <dxgi.h>
#else
    #include "imgui_impl_opengl3.h"
#endif

#include <GLFW/glfw3.h>
#include <Engine/graphics/Window.h>
#include <optional>
#include <string>

// ─────────────────────────────────────────────
//  RendererAvailability — detected at startup
// ─────────────────────────────────────────────
struct RendererAvailability
{
    bool dx11    = false;
    bool opengl  = false;
    bool vulkan  = false;

    std::string dx11Reason;
    std::string vulkanReason;

    static RendererAvailability Detect()
    {
        RendererAvailability a;

#ifdef _WIN32
        // Try creating a D3D11 device to confirm DX11 works
        ID3D11Device* dev = nullptr;
        D3D_FEATURE_LEVEL fl;
        HRESULT hr = D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION, &dev, &fl, nullptr);
        a.dx11 = SUCCEEDED(hr);
        if (!a.dx11) a.dx11Reason = "No DX11-capable GPU found";
        if (dev) dev->Release();
#else
        a.dx11       = false;
        a.dx11Reason = "DirectX 11 is Windows-only";
#endif

        // OpenGL: always available via GLFW/Mesa
        a.opengl = true;

        // Vulkan: check if any ICD is present
#if defined(VK_VERSION_1_0)
        uint32_t count = 0;
        VkResult vr = vkEnumeratePhysicalDevices(VK_NULL_HANDLE, &count, nullptr);
        a.vulkan = (vr == VK_SUCCESS && count > 0);
        if (!a.vulkan) a.vulkanReason = "No Vulkan-capable GPU found";
#else
        a.vulkan       = false;
        a.vulkanReason = "Vulkan SDK not available";
#endif

        return a;
    }
};

// ─────────────────────────────────────────────
//  RendererPicker — small GLFW window with ImGui
// ─────────────────────────────────────────────
class RendererPicker
{
public:
    // Shows the picker window and blocks until the user picks a renderer.
    // Returns the chosen API, or nullopt if the window was closed.
    static std::optional<WindowGraphicsAPI> Show()
    {
        RendererAvailability avail = RendererAvailability::Detect();

        // Pick the best default
        WindowGraphicsAPI defaultChoice =
            avail.dx11   ? WindowGraphicsAPI::DX11   :
            avail.vulkan ? WindowGraphicsAPI::VULKAN  :
                           WindowGraphicsAPI::OpenGL;

        // ── Create a small borderless GLFW/OpenGL window for the picker ──
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        GLFWwindow* win = glfwCreateWindow(420, 300, "Deform — Select Renderer", nullptr, nullptr);
        if (!win) return std::nullopt;

        glfwMakeContextCurrent(win);
        glfwSwapInterval(1);

        // ── ImGui setup (OpenGL3 for the picker always) ──
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(win, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Dark theme
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 6.0f;
        style.FrameRounding  = 4.0f;
        style.GrabRounding   = 4.0f;
        ImVec4* c = style.Colors;
        c[ImGuiCol_WindowBg]      = ImVec4(0.11f, 0.11f, 0.12f, 1.0f);
        c[ImGuiCol_Button]        = ImVec4(0.20f, 0.20f, 0.23f, 1.0f);
        c[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.50f, 0.80f, 1.0f);
        c[ImGuiCol_ButtonActive]  = ImVec4(0.25f, 0.45f, 0.75f, 1.0f);
        c[ImGuiCol_Header]        = ImVec4(0.20f, 0.35f, 0.60f, 1.0f);
        c[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.50f, 0.80f, 1.0f);

        int selected = (defaultChoice == WindowGraphicsAPI::DX11)   ? 0 :
                       (defaultChoice == WindowGraphicsAPI::OpenGL)  ? 1 : 2;

        std::optional<WindowGraphicsAPI> result = std::nullopt;
        bool confirmed = false;

        while (!glfwWindowShouldClose(win) && !confirmed)
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Full-window panel
            ImGui::SetNextWindowPos({0, 0});
            ImGui::SetNextWindowSize({420, 300});
            ImGui::Begin("##picker", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoScrollbar);

            // Title
            ImGui::SetCursorPosY(18);
            ImGui::SetCursorPosX((420 - ImGui::CalcTextSize("Deform Engine").x) * 0.5f);
            ImGui::TextColored({0.4f, 0.7f, 1.0f, 1.0f}, "Deform Engine");

            ImGui::SetCursorPosX((420 - ImGui::CalcTextSize("Select a graphics renderer").x) * 0.5f);
            ImGui::TextDisabled("Select a graphics renderer");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // ── Renderer options ─────────────────────────────────────
            auto RendererOption = [&](int idx, const char* name, const char* desc,
                                      bool available, const std::string& reason)
            {
                bool disabled = !available;
                if (disabled) ImGui::BeginDisabled();

                ImGui::SetCursorPosX(30);

                bool isSelected = (selected == idx);
                if (isSelected)
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.45f, 0.75f, 1.0f));

                ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
                if (ImGui::Selectable(("##sel" + std::to_string(idx)).c_str(),
                                      isSelected, flags, {360, 42}))
                    if (available) selected = idx;

                if (isSelected)
                    ImGui::PopStyleColor();

                // Draw text over the selectable
                ImVec2 pos = ImGui::GetItemRectMin();
                ImGui::GetWindowDrawList()->AddText(
                    {pos.x + 10, pos.y + 6}, IM_COL32(220,220,220,255), name);

                std::string subtext = disabled ? reason : desc;
                ImVec4 subtextColor = disabled
                    ? ImVec4(0.6f, 0.3f, 0.3f, 1.0f)
                    : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

                ImGui::GetWindowDrawList()->AddText(
                    ImGui::GetFont(),
                    ImGui::GetFontSize() * 0.85f,
                    {pos.x + 10, pos.y + 24},
                    ImGui::ColorConvertFloat4ToU32(subtextColor),
                    subtext.c_str());

                if (disabled) ImGui::EndDisabled();
                ImGui::Spacing();
            };

            RendererOption(0, "DirectX 11",
                "Best for Windows. Full feature support.",
                avail.dx11, avail.dx11Reason);

            RendererOption(1, "OpenGL",
                "Cross-platform. Works on Linux, macOS, Windows.",
                avail.opengl, "");

            RendererOption(2, "Vulkan",
                "Low-level. Best performance on modern GPUs.",
                avail.vulkan, avail.vulkanReason);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // ── Confirm button ───────────────────────────────────────
            float btnW = 120.0f;
            ImGui::SetCursorPosX((420 - btnW) * 0.5f);
            if (ImGui::Button("Launch", {btnW, 32}))
            {
                result = (selected == 0) ? WindowGraphicsAPI::DX11   :
                         (selected == 1) ? WindowGraphicsAPI::OpenGL  :
                                           WindowGraphicsAPI::VULKAN;
                confirmed = true;
            }

            ImGui::End();
            ImGui::Render();

            glClearColor(0.11f, 0.11f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(win);
        }

        // ── Cleanup picker ───────────────────────────────────────────
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(win);

        return result;
    }
};
