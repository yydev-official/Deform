#pragma once

#include "PhysicsAPI.h"
#include <Engine/core/maths/vector/Vector.h>

enum class PHYSICS_API ColliderType
{
    AABB,
    Sphere
};

// ─────────────────────────────────────────────
//  AABB Collider
// ─────────────────────────────────────────────
struct PHYSICS_API AABBCollider
{
    Vector3D Min = Vector3D::Zero();
    Vector3D Max = Vector3D::One();

    // Returns center of the AABB
    Vector3D Center() const;

    // Returns half-extents
    Vector3D HalfExtents() const;

    // Test intersection with another AABB
    bool Intersects(const AABBCollider& other) const;

    // Test intersection with a Sphere (forward declared)
    struct SphereCollider;
    bool Intersects(const SphereCollider& sphere) const;

    // Translate by offset
    AABBCollider Translated(const Vector3D& offset) const;
};

// ─────────────────────────────────────────────
//  Sphere Collider
// ─────────────────────────────────────────────
struct PHYSICS_API SphereCollider
{
    Vector3D Center = Vector3D::Zero();
    float    Radius = 0.5f;

    // Test intersection with another Sphere
    bool Intersects(const SphereCollider& other) const;

    // Test intersection with AABB
    bool Intersects(const AABBCollider& aabb) const;

    // Translate by offset
    SphereCollider Translated(const Vector3D& offset) const;
};

// ─────────────────────────────────────────────
//  Collision result
// ─────────────────────────────────────────────
struct PHYSICS_API CollisionResult
{
    bool     Hit            = false;
    Vector3D Normal         = Vector3D::Zero();  // from A to B
    float    PenetrationDepth = 0.0f;
};

// Utility free functions
PHYSICS_API CollisionResult TestAABBvsAABB(const AABBCollider& a, const AABBCollider& b);
PHYSICS_API CollisionResult TestSphereVsSphere(const SphereCollider& a, const SphereCollider& b);
PHYSICS_API CollisionResult TestAABBvsSphere(const AABBCollider& aabb, const SphereCollider& sphere);
