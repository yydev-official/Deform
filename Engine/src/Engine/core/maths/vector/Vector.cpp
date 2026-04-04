// Vector.cpp

#include "Vector.h"

// VECTOR2D
Vector2D::Vector2D() : X(0), Y(0)
{

}

Vector2D::Vector2D(float x, float y) : X(x), Y(y)
{

}

float Vector2D::GetX() const
{
    return X;
}

float Vector2D::GetY() const
{
    return Y;
}

// VECTOR3D
Vector3D::Vector3D() : X(0), Y(0), Z(0)
{

}

Vector3D::Vector3D(float x, float y, float z) : X(x), Y(y), Z(z)
{

}

float Vector3D::GetX() const
{
    return X;
}

float Vector3D::GetY() const
{
    return Y;
}

float Vector3D::GetZ() const
{
    return Z;
}