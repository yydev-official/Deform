#include "../Editor.h"

void Editor::UI::Inspector::UpdateInspector()
{
    ImGui::Begin("Inspector", nullptr);
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