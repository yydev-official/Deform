#pragma once

#include <Engine/EngineAPI.h>
#include <Engine/core/maths/vector/Vector.h>
#include <Engine/core/maths/Quaternion.h>
#include <Engine/core/maths/Matrix4x4.h>

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>

// ─────────────────────────────────────────────
//  Entity ID
// ─────────────────────────────────────────────
using EntityID = uint64_t;
static constexpr EntityID NULL_ENTITY = 0;

// ─────────────────────────────────────────────
//  Component base
// ─────────────────────────────────────────────
struct DEFORM_API Component
{
    EntityID owner = NULL_ENTITY;
    bool     enabled = true;
    virtual ~Component() = default;
};

// ─────────────────────────────────────────────
//  Built-in components
// ─────────────────────────────────────────────

struct DEFORM_API TransformComponent : Component
{
    Vector3D   Position = Vector3D::Zero();
    Quaternion Rotation = Quaternion::Identity();
    Vector3D   Scale    = Vector3D::One();

    Matrix4x4 GetWorldMatrix() const;
};

struct DEFORM_API TagComponent : Component
{
    std::string Name = "Entity";
};

struct DEFORM_API MeshComponent : Component
{
    std::string MeshPath;
    bool        Visible = true;
};

struct DEFORM_API CameraComponent : Component
{
    float FovDegrees  = 60.0f;
    float NearPlane   = 0.1f;
    float FarPlane    = 1000.0f;
    bool  IsMain      = false;

    Matrix4x4 GetProjectionMatrix(float aspect) const;
};

struct DEFORM_API LightComponent : Component
{
    enum class Type { Directional, Point, Spot };

    Type     LightType  = Type::Point;
    Vector3D Color      = {1, 1, 1};
    float    Intensity  = 1.0f;
    float    Range      = 10.0f;    // Point/Spot only
    float    SpotAngle  = 30.0f;    // Spot only
};

// ─────────────────────────────────────────────
//  Entity
// ─────────────────────────────────────────────
struct DEFORM_API Entity
{
    EntityID    ID       = NULL_ENTITY;
    bool        Active   = true;
    EntityID    Parent   = NULL_ENTITY;
    std::vector<EntityID> Children;

    // Component storage (type → component)
    std::unordered_map<std::type_index, std::shared_ptr<Component>> Components;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        comp->owner = ID;
        Components[std::type_index(typeid(T))] = comp;
        return *comp;
    }

    template<typename T>
    T* GetComponent()
    {
        auto it = Components.find(std::type_index(typeid(T)));
        if (it == Components.end()) return nullptr;
        return static_cast<T*>(it->second.get());
    }

    template<typename T>
    bool HasComponent() const
    {
        return Components.count(std::type_index(typeid(T))) > 0;
    }

    template<typename T>
    void RemoveComponent()
    {
        Components.erase(std::type_index(typeid(T)));
    }

    // Shortcut
    std::string GetName() const;
};

// ─────────────────────────────────────────────
//  Scene (ECS registry)
// ─────────────────────────────────────────────
class DEFORM_API Scene
{
public:
    // Lifecycle
    void CreateNewScene();
    void Clear();

    // Entity management
    EntityID  CreateEntity(const std::string& name = "Entity");
    void      DestroyEntity(EntityID id);
    Entity*   GetEntity(EntityID id);
    bool      EntityExists(EntityID id) const;

    // Hierarchy
    void      SetParent(EntityID child, EntityID parent);
    void      RemoveParent(EntityID child);

    // Queries
    std::vector<EntityID>  GetAllEntities()                         const;
    std::vector<EntityID>  GetRootEntities()                        const;

    template<typename T>
    std::vector<EntityID> GetEntitiesWith()
    {
        std::vector<EntityID> result;
        for (auto& [id, entity] : m_entities)
            if (entity.HasComponent<T>())
                result.push_back(id);
        return result;
    }

    // State
    bool     Loaded    = false;
    uint32_t EntityCount() const { return static_cast<uint32_t>(m_entities.size()); }

private:
    std::unordered_map<EntityID, Entity> m_entities;
    EntityID m_nextID = 1;
};
