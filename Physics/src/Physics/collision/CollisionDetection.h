#pragma once

#include <Physics/PhysicsAPI.h>
#include <Physics/collision/Colliders.h>

// ─────────────────────────────────────────────
//  CollisionDetection
//  Tutti i test di intersezione tra forme.
//  Funzioni pure — nessuno stato.
// ─────────────────────────────────────────────
struct PHYSICS_API CollisionDetection
{
    // AABB vs AABB
    static CollisionManifold Test(
        const AABBCollider& a, const PhysVec3& posA,
        const AABBCollider& b, const PhysVec3& posB);

    // Sphere vs Sphere
    static CollisionManifold Test(
        const SphereCollider& a, const PhysVec3& posA,
        const SphereCollider& b, const PhysVec3& posB);

    // AABB vs Sphere
    static CollisionManifold Test(
        const AABBCollider& aabb,  const PhysVec3& posAABB,
        const SphereCollider& sph, const PhysVec3& posSph);
};
