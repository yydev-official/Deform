#pragma once

#include <Physics/PhysicsAPI.h>
#include <cmath>

// ─────────────────────────────────────────────
//  PhysVec3 — standalone (non dipende da Engine)
//  Per usarla con Deform Engine: conversione via
//  PhysVec3 <-> Vector3D negli adapter.
// ─────────────────────────────────────────────
struct PHYSICS_API PhysVec3
{
    float X = 0, Y = 0, Z = 0;

    PhysVec3() = default;
    PhysVec3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    PhysVec3  operator+(const PhysVec3& o) const { return {X+o.X, Y+o.Y, Z+o.Z}; }
    PhysVec3  operator-(const PhysVec3& o) const { return {X-o.X, Y-o.Y, Z-o.Z}; }
    PhysVec3  operator*(float s)           const { return {X*s,   Y*s,   Z*s};   }
    PhysVec3  operator/(float s)           const { return {X/s,   Y/s,   Z/s};   }
    PhysVec3  operator-()                  const { return {-X,    -Y,    -Z};    }
    PhysVec3& operator+=(const PhysVec3& o) { X+=o.X; Y+=o.Y; Z+=o.Z; return *this; }
    PhysVec3& operator-=(const PhysVec3& o) { X-=o.X; Y-=o.Y; Z-=o.Z; return *this; }
    PhysVec3& operator*=(float s)           { X*=s;   Y*=s;   Z*=s;   return *this; }

    float     Dot(const PhysVec3& o)   const { return X*o.X + Y*o.Y + Z*o.Z; }
    PhysVec3  Cross(const PhysVec3& o) const {
        return { Y*o.Z - Z*o.Y, Z*o.X - X*o.Z, X*o.Y - Y*o.X };
    }
    float     LengthSq() const { return X*X + Y*Y + Z*Z; }
    float     Length()   const { return std::sqrt(LengthSq()); }
    PhysVec3  Normalized() const {
        float l = Length();
        return (l > 1e-8f) ? *this / l : PhysVec3{};
    }

    static PhysVec3 Zero()    { return {0,0,0}; }
    static PhysVec3 Up()      { return {0,1,0}; }
    static PhysVec3 Gravity() { return {0,-9.81f,0}; }
};
