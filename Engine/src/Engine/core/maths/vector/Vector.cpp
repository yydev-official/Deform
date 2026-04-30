// Vector.cpp
#include "Vector.h"
#include <cmath>
#include <stdexcept>

// ─────────────────────────────────────────────
//  Vector2D
// ─────────────────────────────────────────────
Vector2D::Vector2D() : X(0), Y(0) {}
Vector2D::Vector2D(float x, float y) : X(x), Y(y) {}

Vector2D  Vector2D::operator+(const Vector2D& o) const { return {X + o.X, Y + o.Y}; }
Vector2D  Vector2D::operator-(const Vector2D& o) const { return {X - o.X, Y - o.Y}; }
Vector2D  Vector2D::operator*(float s)           const { return {X * s,   Y * s};   }
Vector2D  Vector2D::operator/(float s)           const { return {X / s,   Y / s};   }
Vector2D& Vector2D::operator+=(const Vector2D& o) { X += o.X; Y += o.Y; return *this; }
Vector2D& Vector2D::operator-=(const Vector2D& o) { X -= o.X; Y -= o.Y; return *this; }
Vector2D& Vector2D::operator*=(float s)           { X *= s;   Y *= s;   return *this; }
bool      Vector2D::operator==(const Vector2D& o) const { return X == o.X && Y == o.Y; }
bool      Vector2D::operator!=(const Vector2D& o) const { return !(*this == o); }

float    Vector2D::Length()        const { return std::sqrt(X*X + Y*Y); }
float    Vector2D::LengthSquared() const { return X*X + Y*Y; }
float    Vector2D::Dot(const Vector2D& o) const { return X*o.X + Y*o.Y; }

Vector2D Vector2D::Normalized() const
{
    float len = Length();
    if (len < 1e-8f) return {0, 0};
    return {X / len, Y / len};
}

void Vector2D::Normalize()
{
    float len = Length();
    if (len < 1e-8f) { X = Y = 0; return; }
    X /= len; Y /= len;
}

// ─────────────────────────────────────────────
//  Vector3D
// ─────────────────────────────────────────────
Vector3D::Vector3D() : X(0), Y(0), Z(0) {}
Vector3D::Vector3D(float x, float y, float z) : X(x), Y(y), Z(z) {}

Vector3D  Vector3D::operator+(const Vector3D& o) const { return {X+o.X, Y+o.Y, Z+o.Z}; }
Vector3D  Vector3D::operator-(const Vector3D& o) const { return {X-o.X, Y-o.Y, Z-o.Z}; }
Vector3D  Vector3D::operator*(float s)           const { return {X*s,   Y*s,   Z*s};   }
Vector3D  Vector3D::operator/(float s)           const { return {X/s,   Y/s,   Z/s};   }
Vector3D  Vector3D::operator-()                  const { return {-X,    -Y,    -Z};    }
Vector3D& Vector3D::operator+=(const Vector3D& o) { X+=o.X; Y+=o.Y; Z+=o.Z; return *this; }
Vector3D& Vector3D::operator-=(const Vector3D& o) { X-=o.X; Y-=o.Y; Z-=o.Z; return *this; }
Vector3D& Vector3D::operator*=(float s)           { X*=s;   Y*=s;   Z*=s;   return *this; }
bool      Vector3D::operator==(const Vector3D& o) const { return X==o.X && Y==o.Y && Z==o.Z; }
bool      Vector3D::operator!=(const Vector3D& o) const { return !(*this == o); }

float    Vector3D::Length()        const { return std::sqrt(X*X + Y*Y + Z*Z); }
float    Vector3D::LengthSquared() const { return X*X + Y*Y + Z*Z; }
float    Vector3D::Dot(const Vector3D& o)   const { return X*o.X + Y*o.Y + Z*o.Z; }

Vector3D Vector3D::Cross(const Vector3D& o) const
{
    return {
        Y * o.Z - Z * o.Y,
        Z * o.X - X * o.Z,
        X * o.Y - Y * o.X
    };
}

Vector3D Vector3D::Normalized() const
{
    float len = Length();
    if (len < 1e-8f) return {0, 0, 0};
    return {X/len, Y/len, Z/len};
}

void Vector3D::Normalize()
{
    float len = Length();
    if (len < 1e-8f) { X = Y = Z = 0; return; }
    X /= len; Y /= len; Z /= len;
}

float Vector3D::Distance(const Vector3D& a, const Vector3D& b)
{
    return (b - a).Length();
}

Vector3D Vector3D::Lerp(const Vector3D& a, const Vector3D& b, float t)
{
    return a + (b - a) * t;
}
