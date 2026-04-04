// Vector.h

#pragma once

#include <EngineAPI.h>

// Two dimensional vector (X, Y) 
struct DEFORM_API Vector2D
{

    // CONSTRUCTOR AND ITS OVERLOADS
    Vector2D();
    Vector2D(float x, float y);
    
    // VARIABLES

    float X, Y;

    // GETTER METHODS

    float GetX() const;
    
    float GetY() const;

};

// Three dimensional vector (X, Y, Z)
struct DEFORM_API Vector3D
{

    // CONSTRUCTOR AND ITS OVERLOADS

    Vector3D();
    Vector3D(float x, float y, float z);

    // VARIABLES

    float X, Y, Z;

    // GETTER METHODS
    float GetX() const;

    float GetY() const;
    
    float GetZ() const;

};