#pragma once

#include <Physics/PhysicsAPI.h>
#include <Physics/PhysMath.h>

// ─────────────────────────────────────────────
//  Collider type tag
// ─────────────────────────────────────────────
enum class ColliderType { AABB, Sphere };

// ─────────────────────────────────────────────
//  AABB Collider
//  Axis-Aligned Bounding Box — il più veloce
//  da testare. Offset relativo alla posizione
//  del rigidbody.
// ─────────────────────────────────────────────
struct PHYSICS_API AABBCollider
{
    PhysVec3 Offset   = PhysVec3::Zero();  // centro relativo
    PhysVec3 HalfSize = {0.5f, 0.5f, 0.5f}; // semi-dimensioni (es. 1x1x1 box)

    // Min/Max in world space dato il centro dell'oggetto
    PhysVec3 WorldMin(const PhysVec3& center) const;
    PhysVec3 WorldMax(const PhysVec3& center) const;
};

// ─────────────────────────────────────────────
//  Sphere Collider
// ─────────────────────────────────────────────
struct PHYSICS_API SphereCollider
{
    PhysVec3 Offset = PhysVec3::Zero();
    float    Radius = 0.5f;

    PhysVec3 WorldCenter(const PhysVec3& position) const;
};

// ─────────────────────────────────────────────
//  Collision manifold — risultato di un test
// ─────────────────────────────────────────────
struct PHYSICS_API CollisionManifold
{
    bool      HasCollision = false;
    PhysVec3  Normal       = PhysVec3::Zero();  // da B verso A
    float     Penetration  = 0.0f;              // profondità sovrapposizione
    PhysVec3  ContactPoint = PhysVec3::Zero();
};
