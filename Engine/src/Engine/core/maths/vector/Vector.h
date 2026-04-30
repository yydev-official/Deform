// Vector.h
#pragma once

#include <Engine/EngineAPI.h>
#include <cmath>

// ─────────────────────────────────────────────
//  Vector2D
// ─────────────────────────────────────────────
struct DEFORM_API Vector2D
{
    float X, Y;

    Vector2D();
    Vector2D(float x, float y);

    // Arithmetic
    Vector2D  operator+(const Vector2D& o) const;
    Vector2D  operator-(const Vector2D& o) const;
    Vector2D  operator*(float scalar)      const;
    Vector2D  operator/(float scalar)      const;
    Vector2D& operator+=(const Vector2D& o);
    Vector2D& operator-=(const Vector2D& o);
    Vector2D& operator*=(float scalar);

    // Comparison
    bool operator==(const Vector2D& o) const;
    bool operator!=(const Vector2D& o) const;

    // Math
    float     Length()                        const;
    float     LengthSquared()                 const;
    Vector2D  Normalized()                    const;
    void      Normalize();
    float     Dot(const Vector2D& o)          const;

    // Constants
    static Vector2D Zero()  { return {0, 0}; }
    static Vector2D One()   { return {1, 1}; }
    static Vector2D Up()    { return {0, 1}; }
    static Vector2D Right() { return {1, 0}; }
};

// ─────────────────────────────────────────────
//  Vector3D
// ─────────────────────────────────────────────
struct DEFORM_API Vector3D
{
    float X, Y, Z;

    Vector3D();
    Vector3D(float x, float y, float z);

    // Arithmetic
    Vector3D  operator+(const Vector3D& o) const;
    Vector3D  operator-(const Vector3D& o) const;
    Vector3D  operator*(float scalar)      const;
    Vector3D  operator/(float scalar)      const;
    Vector3D& operator+=(const Vector3D& o);
    Vector3D& operator-=(const Vector3D& o);
    Vector3D& operator*=(float scalar);
    Vector3D  operator-()                  const;   // unary negate

    // Comparison
    bool operator==(const Vector3D& o) const;
    bool operator!=(const Vector3D& o) const;

    // Math
    float     Length()                        const;
    float     LengthSquared()                 const;
    Vector3D  Normalized()                    const;
    void      Normalize();
    float     Dot(const Vector3D& o)          const;
    Vector3D  Cross(const Vector3D& o)        const;
    static float     Distance(const Vector3D& a, const Vector3D& b);
    static Vector3D  Lerp(const Vector3D& a, const Vector3D& b, float t);

    // Constants
    static Vector3D Zero()    { return {0, 0, 0}; }
    static Vector3D One()     { return {1, 1, 1}; }
    static Vector3D Up()      { return {0, 1, 0}; }
    static Vector3D Down()    { return {0,-1, 0}; }
    static Vector3D Forward() { return {0, 0,-1}; }
    static Vector3D Back()    { return {0, 0, 1}; }
    static Vector3D Right()   { return {1, 0, 0}; }
    static Vector3D Left()    { return {-1,0, 0}; }
};
