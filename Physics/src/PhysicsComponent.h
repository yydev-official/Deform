#pragma once

#include "PhysicsAPI.h"
#include "Rigidbody.h"
#include "Collider.h"
#include <Engine/core/ecs/ECS.h>

#include <variant>
#include <optional>

// ─────────────────────────────────────────────
//  RigidbodyComponent — aggiunto all'Entity ECS
// ─────────────────────────────────────────────
struct PHYSICS_API RigidbodyComponent : Component
{
    Rigidbody Body;
};

// ─────────────────────────────────────────────
//  ColliderComponent — aggiunto all'Entity ECS
// ─────────────────────────────────────────────
struct PHYSICS_API ColliderComponent : Component
{
    ColliderType Type = ColliderType::AABB;

    // Only one is active based on Type
    AABBCollider   AABB;
    SphereCollider Sphere;

    bool IsTrigger = false;  // trigger = detects but no collision response

    // Helper: get world-space AABB translated by position
    AABBCollider   GetWorldAABB(const Vector3D& position)   const;
    SphereCollider GetWorldSphere(const Vector3D& position) const;
};
