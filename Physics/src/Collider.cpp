#include "Collider.h"
#include <cmath>
#include <algorithm>

// ─────────────────────────────────────────────
//  AABBCollider
// ─────────────────────────────────────────────
Vector3D AABBCollider::Center() const
{
    return {
        (Min.X + Max.X) * 0.5f,
        (Min.Y + Max.Y) * 0.5f,
        (Min.Z + Max.Z) * 0.5f
    };
}

Vector3D AABBCollider::HalfExtents() const
{
    return {
        (Max.X - Min.X) * 0.5f,
        (Max.Y - Min.Y) * 0.5f,
        (Max.Z - Min.Z) * 0.5f
    };
}

bool AABBCollider::Intersects(const AABBCollider& other) const
{
    return (Min.X <= other.Max.X && Max.X >= other.Min.X) &&
           (Min.Y <= other.Max.Y && Max.Y >= other.Min.Y) &&
           (Min.Z <= other.Max.Z && Max.Z >= other.Min.Z);
}

bool AABBCollider::Intersects(const SphereCollider& sphere) const
{
    return TestAABBvsSphere(*this, sphere).Hit;
}

AABBCollider AABBCollider::Translated(const Vector3D& offset) const
{
    return { Min + offset, Max + offset };
}

// ─────────────────────────────────────────────
//  SphereCollider
// ─────────────────────────────────────────────
bool SphereCollider::Intersects(const SphereCollider& other) const
{
    return TestSphereVsSphere(*this, other).Hit;
}

bool SphereCollider::Intersects(const AABBCollider& aabb) const
{
    return TestAABBvsSphere(aabb, *this).Hit;
}

SphereCollider SphereCollider::Translated(const Vector3D& offset) const
{
    return { Center + offset, Radius };
}

// ─────────────────────────────────────────────
//  Collision tests
// ─────────────────────────────────────────────
CollisionResult TestAABBvsAABB(const AABBCollider& a, const AABBCollider& b)
{
    CollisionResult result;

    float overlapX = std::min(a.Max.X, b.Max.X) - std::max(a.Min.X, b.Min.X);
    float overlapY = std::min(a.Max.Y, b.Max.Y) - std::max(a.Min.Y, b.Min.Y);
    float overlapZ = std::min(a.Max.Z, b.Max.Z) - std::max(a.Min.Z, b.Min.Z);

    if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0)
        return result; // no hit

    result.Hit = true;

    // Find axis of minimum penetration
    if (overlapX < overlapY && overlapX < overlapZ)
    {
        result.PenetrationDepth = overlapX;
        result.Normal = (a.Center().X < b.Center().X)
            ? Vector3D{-1, 0, 0} : Vector3D{1, 0, 0};
    }
    else if (overlapY < overlapZ)
    {
        result.PenetrationDepth = overlapY;
        result.Normal = (a.Center().Y < b.Center().Y)
            ? Vector3D{0, -1, 0} : Vector3D{0, 1, 0};
    }
    else
    {
        result.PenetrationDepth = overlapZ;
        result.Normal = (a.Center().Z < b.Center().Z)
            ? Vector3D{0, 0, -1} : Vector3D{0, 0, 1};
    }

    return result;
}

CollisionResult TestSphereVsSphere(const SphereCollider& a, const SphereCollider& b)
{
    CollisionResult result;

    Vector3D diff   = b.Center - a.Center;
    float    distSq = diff.LengthSquared();
    float    radSum = a.Radius + b.Radius;

    if (distSq >= radSum * radSum)
        return result; // no hit

    result.Hit = true;
    float dist = std::sqrt(distSq);

    if (dist < 1e-8f)
    {
        result.Normal           = Vector3D::Up();
        result.PenetrationDepth = radSum;
    }
    else
    {
        result.Normal           = diff * (1.0f / dist); // normalized
        result.PenetrationDepth = radSum - dist;
    }

    return result;
}

CollisionResult TestAABBvsSphere(const AABBCollider& aabb, const SphereCollider& sphere)
{
    CollisionResult result;

    // Closest point on AABB to sphere center
    Vector3D closest = {
        std::max(aabb.Min.X, std::min(sphere.Center.X, aabb.Max.X)),
        std::max(aabb.Min.Y, std::min(sphere.Center.Y, aabb.Max.Y)),
        std::max(aabb.Min.Z, std::min(sphere.Center.Z, aabb.Max.Z))
    };

    Vector3D diff   = sphere.Center - closest;
    float    distSq = diff.LengthSquared();

    if (distSq >= sphere.Radius * sphere.Radius)
        return result; // no hit

    result.Hit = true;
    float dist = std::sqrt(distSq);

    if (dist < 1e-8f)
    {
        result.Normal           = Vector3D::Up();
        result.PenetrationDepth = sphere.Radius;
    }
    else
    {
        result.Normal           = diff * (1.0f / dist);
        result.PenetrationDepth = sphere.Radius - dist;
    }

    return result;
}
