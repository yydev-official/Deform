#pragma once

#include "PhysicsAPI.h"
#include "PhysicsComponent.h"
#include <Engine/core/ecs/ECS.h>

#include <vector>
#include <functional>

struct PHYSICS_API CollisionEvent
{
    EntityID      EntityA;
    EntityID      EntityB;
    CollisionResult Result;
};

using CollisionCallback = std::function<void(const CollisionEvent&)>;

class PHYSICS_API PhysicsWorld
{
public:
    PhysicsWorld();
    ~PhysicsWorld() = default;

    // Settings
    Vector3D Gravity    = {0.0f, -9.81f, 0.0f};
    int      SubSteps   = 2;      // physics substeps per frame (more = more accurate)

    // Step the physics simulation
    // scene: the ECS scene to update
    // deltaTime: time in seconds since last frame
    void Step(Scene& scene, float deltaTime);

    // Register a callback for collision events
    void OnCollision(CollisionCallback callback);

    // Clear all callbacks
    void ClearCallbacks();

private:
    void IntegrateVelocities(Scene& scene, float dt);
    void DetectAndResolveCollisions(Scene& scene);
    void ResolveCollision(Entity& a, Entity& b, const CollisionResult& result);

    std::vector<CollisionCallback> m_callbacks;

    void FireCollisionEvent(EntityID a, EntityID b, const CollisionResult& result);
};
