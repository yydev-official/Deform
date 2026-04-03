#include "../Editor.h"

using namespace Editor;
using deform::Logger;

void UI::MainBar::UpdateMainBar(UIState& state) 
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                Logger::Log("New Scene Created!");
            }

            if (ImGui::MenuItem("Open..."))
            {
                Logger::Log("New Scene Opened!");
            }

            if (ImGui::MenuItem("Save"))
            {
                Logger::Log("Scene Saved!");
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Hierarchy", nullptr, &state.showHierarchy);
            ImGui::MenuItem("Inspector", nullptr, &state.showInspector);
            ImGui::MenuItem("Scene", nullptr, &state.showScene);
            ImGui::MenuItem("Console", nullptr, &state.showConsole);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Render"))
        {
            ImGui::ColorEdit4("Clear Color", state.clearColor);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
