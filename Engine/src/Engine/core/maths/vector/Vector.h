// Vector.h
#pragma once

#include <Engine/EngineAPI.h>

// Two dimensional vector (X, Y)
struct DEFORM_API Vector2D
{
    Vector2D();
    Vector2D(float x, float y);
    float X, Y;
};

// Three dimensional vector (X, Y, Z)
struct DEFORM_API Vector3D
{
    Vector3D();
    Vector3D(float x, float y, float z);
    float X, Y, Z;
};
