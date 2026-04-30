#include "Quaternion.h"
#include <cmath>

static constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;
static constexpr float RAD2DEG = 180.0f / 3.14159265358979323846f;

Quaternion::Quaternion() : X(0), Y(0), Z(0), W(1) {}
Quaternion::Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

Quaternion Quaternion::Identity() { return {0, 0, 0, 1}; }

Quaternion Quaternion::operator*(const Quaternion& o) const
{
    return {
        W*o.X + X*o.W + Y*o.Z - Z*o.Y,
        W*o.Y - X*o.Z + Y*o.W + Z*o.X,
        W*o.Z + X*o.Y - Y*o.X + Z*o.W,
        W*o.W - X*o.X - Y*o.Y - Z*o.Z
    };
}

Vector3D Quaternion::operator*(const Vector3D& v) const
{
    Vector3D qv = {X, Y, Z};
    Vector3D t  = qv.Cross(v) * 2.0f;
    return v + t * W + qv.Cross(t);
}

Quaternion Quaternion::operator-() const { return {-X, -Y, -Z, -W}; }

float      Quaternion::Length()     const { return std::sqrt(X*X+Y*Y+Z*Z+W*W); }
Quaternion Quaternion::Conjugate()  const { return {-X, -Y, -Z, W}; }

Quaternion Quaternion::Normalized() const
{
    float l = Length();
    if (l < 1e-8f) return Identity();
    return {X/l, Y/l, Z/l, W/l};
}

void Quaternion::Normalize()
{
    float l = Length();
    if (l < 1e-8f) { *this = Identity(); return; }
    X/=l; Y/=l; Z/=l; W/=l;
}

Quaternion Quaternion::Inverse() const
{
    float l2 = X*X+Y*Y+Z*Z+W*W;
    if (l2 < 1e-8f) return Identity();
    return {-X/l2, -Y/l2, -Z/l2, W/l2};
}

Quaternion Quaternion::FromAxisAngle(const Vector3D& axis, float radians)
{
    float half = radians * 0.5f;
    float s    = std::sin(half);
    Vector3D a = axis.Normalized();
    return {a.X*s, a.Y*s, a.Z*s, std::cos(half)};
}

Quaternion Quaternion::FromEulerDegrees(float pitch, float yaw, float roll)
{
    float p = pitch * DEG2RAD * 0.5f;
    float y = yaw   * DEG2RAD * 0.5f;
    float r = roll  * DEG2RAD * 0.5f;

    Quaternion qp = {std::sin(p), 0, 0, std::cos(p)};
    Quaternion qy = {0, std::sin(y), 0, std::cos(y)};
    Quaternion qr = {0, 0, std::sin(r), std::cos(r)};
    return (qy * qp * qr).Normalized();
}

Vector3D Quaternion::ToEulerDegrees() const
{
    // pitch (X), yaw (Y), roll (Z) in degrees
    float sinr_cosp = 2.0f * (W*X + Y*Z);
    float cosr_cosp = 1.0f - 2.0f * (X*X + Y*Y);
    float pitch = std::atan2(sinr_cosp, cosr_cosp) * RAD2DEG;

    float sinp = 2.0f * (W*Y - Z*X);
    float yaw = (std::abs(sinp) >= 1.0f)
        ? std::copysign(90.0f, sinp)
        : std::asin(sinp) * RAD2DEG;

    float siny_cosp = 2.0f * (W*Z + X*Y);
    float cosy_cosp = 1.0f - 2.0f * (Y*Y + Z*Z);
    float roll = std::atan2(siny_cosp, cosy_cosp) * RAD2DEG;

    return {pitch, yaw, roll};
}

Matrix4x4 Quaternion::ToMatrix() const
{
    Quaternion q = Normalized();
    Matrix4x4 r = Matrix4x4::Identity();

    r.m[0][0] = 1 - 2*(q.Y*q.Y + q.Z*q.Z);
    r.m[0][1] =     2*(q.X*q.Y + q.Z*q.W);
    r.m[0][2] =     2*(q.X*q.Z - q.Y*q.W);

    r.m[1][0] =     2*(q.X*q.Y - q.Z*q.W);
    r.m[1][1] = 1 - 2*(q.X*q.X + q.Z*q.Z);
    r.m[1][2] =     2*(q.Y*q.Z + q.X*q.W);

    r.m[2][0] =     2*(q.X*q.Z + q.Y*q.W);
    r.m[2][1] =     2*(q.Y*q.Z - q.X*q.W);
    r.m[2][2] = 1 - 2*(q.X*q.X + q.Y*q.Y);

    return r;
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
    float dot = a.X*b.X + a.Y*b.Y + a.Z*b.Z + a.W*b.W;
    Quaternion b2 = (dot < 0.0f) ? -b : b;
    dot = std::abs(dot);

    if (dot > 0.9995f) {
        // Linear interpolation for very close quaternions
        Quaternion r = {
            a.X + t*(b2.X - a.X),
            a.Y + t*(b2.Y - a.Y),
            a.Z + t*(b2.Z - a.Z),
            a.W + t*(b2.W - a.W)
        };
        return r.Normalized();
    }

    float theta0 = std::acos(dot);
    float theta  = theta0 * t;
    float s0     = std::cos(theta) - dot * std::sin(theta) / std::sin(theta0);
    float s1     = std::sin(theta) / std::sin(theta0);

    return {
        s0*a.X + s1*b2.X,
        s0*a.Y + s1*b2.Y,
        s0*a.Z + s1*b2.Z,
        s0*a.W + s1*b2.W
    };
}
