
#include "../Editor.h"

DXRender DX11;

bool Editor::UI::Init()
{
	// Initialize ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Set ImGui style
	ImGui::StyleColorsDark();
	return true;
}

void Editor::UI::Update()
{
	// Start a new ImGui frame
// Render to scene texture (use dark gray so it's clearly not white)
    if (DX11.m_sceneRTV)
    {
        float sceneClear[4] = { 0.15f, 0.15f, 0.5f, 1.0f };
        DX11.m_context->OMSetRenderTargets(1, &DX11.m_sceneRTV, nullptr);
        DX11.m_context->ClearRenderTargetView(DX11.m_sceneRTV, sceneClear);
        // TODO: draw your 3D scene here
    }

	ID3D11DepthStencilView* nullDSV = nullptr;

    float clearColor[4] = { 0.05f, 0.5f, 1.0f, 0.0f };
    DX11.m_context->OMSetRenderTargets(1, &DX11.m_rtv, nullptr);
    DX11.m_context->ClearRenderTargetView(DX11.m_rtv, clearColor);


    // Start a new ImGui frame
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

    // This is the actual dockspace
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    ImGui::End();

    // Main menu bar
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
    // Full-window dockspace
    ImGui::DockSpaceOverViewport(main_viewport->ID);

    // Example panels
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
        if (DX11.m_sceneSRV)
            ImGui::Image((ImTextureID)DX11.m_sceneSRV, size);
        else
            ImGui::TextUnformatted("(Scene texture not available)");
        ImGui::End();
    }

    if (showConsole)
    {
        ImGui::Begin("Console", &showConsole);
        ImGui::TextUnformatted("[Log] Editor started");
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    DX11.m_swapChain->Present(1, 0);
}