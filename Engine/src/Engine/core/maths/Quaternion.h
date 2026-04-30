#pragma once

#include <Engine/EngineAPI.h>
#include <Engine/core/maths/vector/Vector.h>
#include <Engine/core/maths/Matrix4x4.h>
#include <cmath>

struct DEFORM_API Quaternion
{
    float X, Y, Z, W;

    Quaternion();
    Quaternion(float x, float y, float z, float w);

    // Operators
    Quaternion operator*(const Quaternion& o) const;
    Vector3D   operator*(const Vector3D& v)   const;
    Quaternion operator-()                    const;

    // Math
    float      Length()     const;
    Quaternion Normalized() const;
    void       Normalize();
    Quaternion Conjugate()  const;
    Quaternion Inverse()    const;

    // Conversion
    Matrix4x4  ToMatrix()   const;
    Vector3D   ToEulerDegrees() const;

    // Static factories
    static Quaternion Identity();
    static Quaternion FromAxisAngle(const Vector3D& axis, float radians);
    static Quaternion FromEulerDegrees(float pitchDeg, float yawDeg, float rollDeg);
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
};
