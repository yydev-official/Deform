#include "../Editor.h"
#include <Engine/core/ecs/ECS.h>

void Editor::UI::Inspector::UpdateInspector(UIState& state)
{
    ImGui::Begin("Inspector", nullptr);

    if (!state.scene || state.selectedEntity == NULL_ENTITY)
    {
        ImGui::TextUnformatted("Select an entity in the Hierarchy.");
        ImGui::End();
        return;
    }

    Entity* entity = state.scene->GetEntity(state.selectedEntity);
    if (!entity)
    {
        ImGui::TextUnformatted("Entity not found.");
        ImGui::End();
        return;
    }

    // ── Tag / Name ──────────────────────────────────────
    if (auto* tag = entity->GetComponent<TagComponent>())
    {
        char buf[256];
        std::strncpy(buf, tag->Name.c_str(), sizeof(buf));
        buf[sizeof(buf)-1] = '\0';
        if (ImGui::InputText("##name", buf, sizeof(buf)))
            tag->Name = buf;
    }

    ImGui::SameLine();
    bool active = entity->Active;
    if (ImGui::Checkbox("Active", &active))
        entity->Active = active;

    ImGui::Separator();

    // ── Transform ───────────────────────────────────────
    if (auto* t = entity->GetComponent<TransformComponent>())
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Position", &t->Position.X, 0.1f);

            Vector3D euler = t->Rotation.ToEulerDegrees();
            if (ImGui::DragFloat3("Rotation", &euler.X, 0.5f))
                t->Rotation = Quaternion::FromEulerDegrees(euler.X, euler.Y, euler.Z);

            ImGui::DragFloat3("Scale", &t->Scale.X, 0.01f, 0.001f, 1000.0f);
        }
    }

    // ── Camera ──────────────────────────────────────────
    if (auto* cam = entity->GetComponent<CameraComponent>())
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("FOV",       &cam->FovDegrees, 0.5f, 1.0f, 179.0f);
            ImGui::DragFloat("Near",      &cam->NearPlane,  0.01f, 0.001f, 10.0f);
            ImGui::DragFloat("Far",       &cam->FarPlane,   1.0f, 1.0f, 100000.0f);
            ImGui::Checkbox("Main Camera", &cam->IsMain);
        }
    }

    // ── Light ───────────────────────────────────────────
    if (auto* light = entity->GetComponent<LightComponent>())
    {
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char* types[] = {"Directional", "Point", "Spot"};
            int typeIdx = static_cast<int>(light->LightType);
            if (ImGui::Combo("Type", &typeIdx, types, 3))
                light->LightType = static_cast<LightComponent::Type>(typeIdx);

            ImGui::ColorEdit3("Color",     &light->Color.X);
            ImGui::DragFloat("Intensity",  &light->Intensity, 0.05f, 0.0f, 100.0f);

            if (light->LightType != LightComponent::Type::Directional)
                ImGui::DragFloat("Range",  &light->Range, 0.1f, 0.0f, 10000.0f);

            if (light->LightType == LightComponent::Type::Spot)
                ImGui::DragFloat("Spot Angle", &light->SpotAngle, 0.5f, 1.0f, 179.0f);
        }
    }

    // ── Mesh ────────────────────────────────────────────
    if (auto* mesh = entity->GetComponent<MeshComponent>())
    {
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Mesh: %s", mesh->MeshPath.empty() ? "(none)" : mesh->MeshPath.c_str());
            ImGui::Checkbox("Visible", &mesh->Visible);
        }
    }

    // ── Add Component ───────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    if (ImGui::Button("Add Component", {-1, 0}))
        ImGui::OpenPopup("add_component_popup");

    if (ImGui::BeginPopup("add_component_popup"))
    {
        if (!entity->HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
            entity->AddComponent<CameraComponent>();
        if (!entity->HasComponent<LightComponent>() && ImGui::MenuItem("Light"))
            entity->AddComponent<LightComponent>();
        if (!entity->HasComponent<MeshComponent>() && ImGui::MenuItem("Mesh"))
            entity->AddComponent<MeshComponent>();
        ImGui::EndPopup();
    }

    ImGui::End();
}