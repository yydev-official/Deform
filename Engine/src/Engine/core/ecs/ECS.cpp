#include "ECS.h"
#include <cmath>

// ─────────────────────────────────────────────
//  TransformComponent
// ─────────────────────────────────────────────
Matrix4x4 TransformComponent::GetWorldMatrix() const
{
    return Matrix4x4::Translate(Position)
         * Rotation.ToMatrix()
         * Matrix4x4::Scale(Scale);
}

// ─────────────────────────────────────────────
//  CameraComponent
// ─────────────────────────────────────────────
Matrix4x4 CameraComponent::GetProjectionMatrix(float aspect) const
{
    float fovRad = FovDegrees * (3.14159265f / 180.0f);
    return Matrix4x4::Perspective(fovRad, aspect, NearPlane, FarPlane);
}

// ─────────────────────────────────────────────
//  Entity
// ─────────────────────────────────────────────
std::string Entity::GetName() const
{
    auto it = Components.find(std::type_index(typeid(TagComponent)));
    if (it == Components.end()) return "Entity";
    return static_cast<TagComponent*>(it->second.get())->Name;
}

// ─────────────────────────────────────────────
//  Scene
// ─────────────────────────────────────────────
void Scene::CreateNewScene()
{
    Clear();

    // Default camera
    EntityID camID = CreateEntity("Camera");
    Entity* cam = GetEntity(camID);
    auto& camComp = cam->AddComponent<CameraComponent>();
    camComp.IsMain = true;
    auto& camTransform = cam->AddComponent<TransformComponent>();
    camTransform.Position = {0, 1, 5};

    // Default directional light
    EntityID lightID = CreateEntity("Directional Light");
    Entity* light = GetEntity(lightID);
    auto& lightComp = light->AddComponent<LightComponent>();
    lightComp.LightType = LightComponent::Type::Directional;
    lightComp.Intensity = 1.0f;
    auto& lightTransform = light->AddComponent<TransformComponent>();
    lightTransform.Rotation = Quaternion::FromEulerDegrees(45.0f, -30.0f, 0.0f);

    Loaded = true;
}

void Scene::Clear()
{
    m_entities.clear();
    m_nextID = 1;
    Loaded = false;
}

EntityID Scene::CreateEntity(const std::string& name)
{
    EntityID id = m_nextID++;
    Entity& entity = m_entities[id];
    entity.ID = id;
    entity.Active = true;
    entity.AddComponent<TagComponent>().Name = name;
    entity.AddComponent<TransformComponent>();
    return id;
}

void Scene::DestroyEntity(EntityID id)
{
    Entity* e = GetEntity(id);
    if (!e) return;

    // Detach children
    for (EntityID childID : e->Children)
    {
        Entity* child = GetEntity(childID);
        if (child) child->Parent = NULL_ENTITY;
    }

    // Remove from parent
    if (e->Parent != NULL_ENTITY)
    {
        Entity* parent = GetEntity(e->Parent);
        if (parent)
        {
            auto& ch = parent->Children;
            ch.erase(std::remove(ch.begin(), ch.end(), id), ch.end());
        }
    }

    m_entities.erase(id);
}

Entity* Scene::GetEntity(EntityID id)
{
    auto it = m_entities.find(id);
    return (it != m_entities.end()) ? &it->second : nullptr;
}

bool Scene::EntityExists(EntityID id) const
{
    return m_entities.count(id) > 0;
}

void Scene::SetParent(EntityID child, EntityID parent)
{
    Entity* c = GetEntity(child);
    Entity* p = GetEntity(parent);
    if (!c || !p || child == parent) return;

    RemoveParent(child);
    c->Parent = parent;
    p->Children.push_back(child);
}

void Scene::RemoveParent(EntityID child)
{
    Entity* c = GetEntity(child);
    if (!c || c->Parent == NULL_ENTITY) return;

    Entity* p = GetEntity(c->Parent);
    if (p)
    {
        auto& ch = p->Children;
        ch.erase(std::remove(ch.begin(), ch.end(), child), ch.end());
    }
    c->Parent = NULL_ENTITY;
}

std::vector<EntityID> Scene::GetAllEntities() const
{
    std::vector<EntityID> result;
    result.reserve(m_entities.size());
    for (auto& [id, _] : m_entities) result.push_back(id);
    return result;
}

std::vector<EntityID> Scene::GetRootEntities() const
{
    std::vector<EntityID> result;
    for (auto& [id, entity] : m_entities)
        if (entity.Parent == NULL_ENTITY)
            result.push_back(id);
    return result;
}
