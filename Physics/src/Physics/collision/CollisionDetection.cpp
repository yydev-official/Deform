#include "CollisionDetection.h"
#include <cmath>
#include <algorithm>

// ─── AABBCollider ────────────────────────────
PhysVec3 AABBCollider::WorldMin(const PhysVec3& center) const
{
    PhysVec3 c = center + Offset;
    return { c.X - HalfSize.X, c.Y - HalfSize.Y, c.Z - HalfSize.Z };
}

PhysVec3 AABBCollider::WorldMax(const PhysVec3& center) const
{
    PhysVec3 c = center + Offset;
    return { c.X + HalfSize.X, c.Y + HalfSize.Y, c.Z + HalfSize.Z };
}

// ─── SphereCollider ──────────────────────────
PhysVec3 SphereCollider::WorldCenter(const PhysVec3& position) const
{
    return position + Offset;
}

// ─── AABB vs AABB ────────────────────────────
CollisionManifold CollisionDetection::Test(
    const AABBCollider& a, const PhysVec3& posA,
    const AABBCollider& b, const PhysVec3& posB)
{
    CollisionManifold m;

    PhysVec3 minA = a.WorldMin(posA), maxA = a.WorldMax(posA);
    PhysVec3 minB = b.WorldMin(posB), maxB = b.WorldMax(posB);

    // SAT su 3 assi
    float overlapX = std::min(maxA.X, maxB.X) - std::max(minA.X, minB.X);
    float overlapY = std::min(maxA.Y, maxB.Y) - std::max(minA.Y, minB.Y);
    float overlapZ = std::min(maxA.Z, maxB.Z) - std::max(minA.Z, minB.Z);

    if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0)
        return m; // no collision

    m.HasCollision = true;

    // Asse di penetrazione minima
    if (overlapX < overlapY && overlapX < overlapZ)
    {
        m.Penetration = overlapX;
        m.Normal = (posA.X < posB.X) ? PhysVec3{-1,0,0} : PhysVec3{1,0,0};
    }
    else if (overlapY < overlapZ)
    {
        m.Penetration = overlapY;
        m.Normal = (posA.Y < posB.Y) ? PhysVec3{0,-1,0} : PhysVec3{0,1,0};
    }
    else
    {
        m.Penetration = overlapZ;
        m.Normal = (posA.Z < posB.Z) ? PhysVec3{0,0,-1} : PhysVec3{0,0,1};
    }

    // Punto di contatto approssimato al centro della sovrapposizione
    m.ContactPoint = {
        (std::max(minA.X, minB.X) + std::min(maxA.X, maxB.X)) * 0.5f,
        (std::max(minA.Y, minB.Y) + std::min(maxA.Y, maxB.Y)) * 0.5f,
        (std::max(minA.Z, minB.Z) + std::min(maxA.Z, maxB.Z)) * 0.5f
    };

    return m;
}

// ─── Sphere vs Sphere ────────────────────────
CollisionManifold CollisionDetection::Test(
    const SphereCollider& a, const PhysVec3& posA,
    const SphereCollider& b, const PhysVec3& posB)
{
    CollisionManifold m;

    PhysVec3 cA = a.WorldCenter(posA);
    PhysVec3 cB = b.WorldCenter(posB);

    PhysVec3 delta      = cA - cB;
    float    distSq     = delta.LengthSq();
    float    radSum     = a.Radius + b.Radius;

    if (distSq >= radSum * radSum) return m;

    m.HasCollision = true;
    float dist = std::sqrt(distSq);

    if (dist < 1e-8f)
    {
        // Centri coincidenti — scegli asse arbitrario
        m.Normal      = PhysVec3::Up();
        m.Penetration = radSum;
    }
    else
    {
        m.Normal      = delta / dist;  // da B verso A
        m.Penetration = radSum - dist;
    }

    m.ContactPoint = cB + m.Normal * b.Radius;
    return m;
}

// ─── AABB vs Sphere ──────────────────────────
CollisionManifold CollisionDetection::Test(
    const AABBCollider&  aabb,   const PhysVec3& posAABB,
    const SphereCollider& sph,   const PhysVec3& posSph)
{
    CollisionManifold m;

    PhysVec3 minA = aabb.WorldMin(posAABB);
    PhysVec3 maxA = aabb.WorldMax(posAABB);
    PhysVec3 sc   = sph.WorldCenter(posSph);

    // Punto più vicino sull'AABB alla sfera (clamped)
    PhysVec3 closest = {
        std::max(minA.X, std::min(sc.X, maxA.X)),
        std::max(minA.Y, std::min(sc.Y, maxA.Y)),
        std::max(minA.Z, std::min(sc.Z, maxA.Z))
    };

    PhysVec3 delta  = sc - closest;
    float    distSq = delta.LengthSq();

    if (distSq >= sph.Radius * sph.Radius) return m;

    m.HasCollision = true;
    float dist = std::sqrt(distSq);

    if (dist < 1e-8f)
    {
        m.Normal      = PhysVec3::Up();
        m.Penetration = sph.Radius;
    }
    else
    {
        m.Normal      = delta / dist;
        m.Penetration = sph.Radius - dist;
    }

    m.ContactPoint = closest;
    return m;
}
