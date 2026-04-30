#include "../Editor.h"
#include <Engine/core/ecs/ECS.h>

void Editor::UI::Hierarchy::UpdateHierarchy(UIState& state)
{
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);

    // Toolbar
    if (ImGui::Button("+ Entity") && state.scene)
        state.scene->CreateEntity("New Entity");

    ImGui::Separator();

    if (!state.scene || !state.scene->Loaded)
    {
        ImGui::TextUnformatted("No scene loaded.");
        ImGui::End();
        return;
    }

    // Render root entities (recursive)
    std::function<void(EntityID)> drawEntity = [&](EntityID id)
    {
        Entity* entity = state.scene->GetEntity(id);
        if (!entity || !entity->Active) return;

        std::string name = entity->GetName();
        bool hasChildren = !entity->Children.empty();

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth;

        if (!hasChildren)
            flags |= ImGuiTreeNodeFlags_Leaf;

        if (state.selectedEntity == id)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool open = ImGui::TreeNodeEx((void*)(uintptr_t)id, flags, "%s", name.c_str());

        if (ImGui::IsItemClicked())
            state.selectedEntity = id;

        // Right-click context menu
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete"))
            {
                if (state.selectedEntity == id) state.selectedEntity = NULL_ENTITY;
                state.scene->DestroyEntity(id);
                ImGui::EndPopup();
                if (open) ImGui::TreePop();
                return;
            }
            if (ImGui::MenuItem("Duplicate"))
            {
                EntityID newID = state.scene->CreateEntity(name + " (Copy)");
                // Copy transform
                Entity* src = state.scene->GetEntity(id);
                Entity* dst = state.scene->GetEntity(newID);
                if (src && dst)
                {
                    auto* st = src->GetComponent<TransformComponent>();
                    auto* dt = dst->GetComponent<TransformComponent>();
                    if (st && dt) *dt = *st;
                }
            }
            ImGui::EndPopup();
        }

        if (open)
        {
            for (EntityID childID : entity->Children)
                drawEntity(childID);
            ImGui::TreePop();
        }
    };

    for (EntityID id : state.scene->GetRootEntities())
        drawEntity(id);

    ImGui::End();
}