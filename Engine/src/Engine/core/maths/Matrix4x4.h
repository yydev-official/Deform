#pragma once

#include <Engine/EngineAPI.h>
#include <Engine/core/maths/vector/Vector.h>
#include <cmath>
#include <cstring>

// Column-major 4x4 matrix (compatible with HLSL/GLSL cbuffers)
struct DEFORM_API Matrix4x4
{
    float m[4][4]; // m[col][row]

    Matrix4x4();
    explicit Matrix4x4(float diagonal);

    // Operators
    Matrix4x4 operator*(const Matrix4x4& o) const;
    Vector3D  operator*(const Vector3D& v)  const;

    // Static factories
    static Matrix4x4 Identity();
    static Matrix4x4 Translate(const Vector3D& t);
    static Matrix4x4 Scale(const Vector3D& s);
    static Matrix4x4 RotateX(float radians);
    static Matrix4x4 RotateY(float radians);
    static Matrix4x4 RotateZ(float radians);

    // View / Projection
    static Matrix4x4 LookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up);
    static Matrix4x4 Perspective(float fovYRadians, float aspect, float nearZ, float farZ);
    static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ);

    // Utility
    Matrix4x4 Transposed() const;
    const float* Data() const { return &m[0][0]; }
};
