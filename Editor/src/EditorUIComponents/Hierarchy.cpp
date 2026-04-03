#include "../Editor.h"

void Editor::UI::Hierarchy::UpdateHierarchy()
{
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);
    ImGui::TextUnformatted("Scene");
    ImGui::Separator();
    ImGui::Selectable("Camera");
    ImGui::Selectable("Light");
    ImGui::Selectable("Cube");
    ImGui::End();
}