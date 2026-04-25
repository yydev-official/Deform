// Vector.cpp
#include "Vector.h"
#include <vector>

// This is a 2D vector class that represents a point or direction in 2D space. It has two float members, X and Y, which store the horizontal and vertical components of the vector, respectively. The class provides constructors for initializing the vector and getter methods to access its components.
Vector2D::Vector2D() : X(0), Y(0)
{
    // Default constructor initializes the vector to (0, 0)
}

// This is a 2D vector class that represents a point or direction in 2D space. It has two float members, X and Y, which store the horizontal and vertical components of the vector, respectively. The class provides constructors for initializing the vector and getter methods to access its components.
Vector2D::Vector2D(float x, float y) : X(x), Y(y)
{
    // Constructor that initializes the vector to (x, y)
}

// This is a 3D vector class that represents a point or direction in 3D space. It has three float members, X, Y, and Z, which store the horizontal, vertical, and depth components of the vector, respectively. The class provides constructors for initializing the vector and getter methods to access its components.
Vector3D::Vector3D() : X(0), Y(0), Z(0)
{
    // Default constructor initializes the vector to (0, 0, 0)
}

// This is a 3D vector class that represents a point or direction in 3D space. It has three float members, X, Y, and Z, which store the horizontal, vertical, and depth components of the vector, respectively. The class provides constructors for initializing the vector and getter methods to access its components.
Vector3D::Vector3D(float x, float y, float z) : X(x), Y(y), Z(z)
{
    // Constructor that initializes the vector to (x, y, z)
}
