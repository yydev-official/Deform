#include "Matrix4x4.h"
#include <cmath>
#include <cstring>

Matrix4x4::Matrix4x4() { std::memset(m, 0, sizeof(m)); }
Matrix4x4::Matrix4x4(float d)
{
    std::memset(m, 0, sizeof(m));
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = d;
}

Matrix4x4 Matrix4x4::Identity() { return Matrix4x4(1.0f); }

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& o) const
{
    Matrix4x4 r;
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row)
            for (int k = 0; k < 4; ++k)
                r.m[col][row] += m[k][row] * o.m[col][k];
    return r;
}

Vector3D Matrix4x4::operator*(const Vector3D& v) const
{
    float x = m[0][0]*v.X + m[1][0]*v.Y + m[2][0]*v.Z + m[3][0];
    float y = m[0][1]*v.X + m[1][1]*v.Y + m[2][1]*v.Z + m[3][1];
    float z = m[0][2]*v.X + m[1][2]*v.Y + m[2][2]*v.Z + m[3][2];
    float w = m[0][3]*v.X + m[1][3]*v.Y + m[2][3]*v.Z + m[3][3];
    if (std::abs(w) > 1e-8f) { x/=w; y/=w; z/=w; }
    return {x, y, z};
}

Matrix4x4 Matrix4x4::Translate(const Vector3D& t)
{
    Matrix4x4 r = Identity();
    r.m[3][0] = t.X; r.m[3][1] = t.Y; r.m[3][2] = t.Z;
    return r;
}

Matrix4x4 Matrix4x4::Scale(const Vector3D& s)
{
    Matrix4x4 r = Identity();
    r.m[0][0] = s.X; r.m[1][1] = s.Y; r.m[2][2] = s.Z;
    return r;
}

Matrix4x4 Matrix4x4::RotateX(float a)
{
    Matrix4x4 r = Identity();
    r.m[1][1] =  std::cos(a); r.m[2][1] = -std::sin(a);
    r.m[1][2] =  std::sin(a); r.m[2][2] =  std::cos(a);
    return r;
}

Matrix4x4 Matrix4x4::RotateY(float a)
{
    Matrix4x4 r = Identity();
    r.m[0][0] =  std::cos(a); r.m[2][0] =  std::sin(a);
    r.m[0][2] = -std::sin(a); r.m[2][2] =  std::cos(a);
    return r;
}

Matrix4x4 Matrix4x4::RotateZ(float a)
{
    Matrix4x4 r = Identity();
    r.m[0][0] =  std::cos(a); r.m[1][0] = -std::sin(a);
    r.m[0][1] =  std::sin(a); r.m[1][1] =  std::cos(a);
    return r;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up)
{
    Vector3D f = (center - eye).Normalized();
    Vector3D r = f.Cross(up).Normalized();
    Vector3D u = r.Cross(f);

    Matrix4x4 mat = Identity();
    mat.m[0][0] =  r.X; mat.m[1][0] =  r.Y; mat.m[2][0] =  r.Z;
    mat.m[0][1] =  u.X; mat.m[1][1] =  u.Y; mat.m[2][1] =  u.Z;
    mat.m[0][2] = -f.X; mat.m[1][2] = -f.Y; mat.m[2][2] = -f.Z;
    mat.m[3][0] = -r.Dot(eye);
    mat.m[3][1] = -u.Dot(eye);
    mat.m[3][2] =  f.Dot(eye);
    return mat;
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float nearZ, float farZ)
{
    float tanHalf = std::tan(fov / 2.0f);
    Matrix4x4 r;
    r.m[0][0] = 1.0f / (aspect * tanHalf);
    r.m[1][1] = 1.0f / tanHalf;
    r.m[2][2] = farZ / (nearZ - farZ);
    r.m[2][3] = -1.0f;
    r.m[3][2] = -(farZ * nearZ) / (farZ - nearZ);
    return r;
}

Matrix4x4 Matrix4x4::Orthographic(float l, float r2, float b, float t, float n, float f)
{
    Matrix4x4 mat = Identity();
    mat.m[0][0] =  2.0f / (r2 - l);
    mat.m[1][1] =  2.0f / (t - b);
    mat.m[2][2] = -2.0f / (f - n);
    mat.m[3][0] = -(r2 + l) / (r2 - l);
    mat.m[3][1] = -(t + b)  / (t - b);
    mat.m[3][2] = -(f + n)  / (f - n);
    return mat;
}

Matrix4x4 Matrix4x4::Transposed() const
{
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            r.m[i][j] = m[j][i];
    return r;
}
