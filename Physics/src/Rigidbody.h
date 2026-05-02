#pragma once

#include "PhysicsAPI.h"
#include <Engine/core/maths/vector/Vector.h>

struct PHYSICS_API Rigidbody
{
    // Linear
    Vector3D Velocity         = Vector3D::Zero();
    Vector3D Acceleration     = Vector3D::Zero();
    Vector3D Force            = Vector3D::Zero();  // accumulated this frame

    // Angular
    Vector3D AngularVelocity  = Vector3D::Zero();
    Vector3D Torque           = Vector3D::Zero();

    // Properties
    float    Mass             = 1.0f;    // kg
    float    Drag             = 0.01f;   // linear damping
    float    AngularDrag      = 0.05f;   // angular damping
    float    Restitution      = 0.3f;    // bounciness [0-1]
    float    Friction         = 0.5f;    // surface friction [0-1]
    bool     UseGravity       = true;
    bool     IsKinematic      = false;   // if true, not affected by forces
    bool     FreezePositionX  = false;
    bool     FreezePositionY  = false;
    bool     FreezePositionZ  = false;

    // Computed
    float InverseMass() const { return (Mass > 1e-8f) ? 1.0f / Mass : 0.0f; }

    // Apply a force at center of mass
    void AddForce(const Vector3D& force);

    // Apply an impulse (immediate velocity change)
    void AddImpulse(const Vector3D& impulse);

    // Apply a torque
    void AddTorque(const Vector3D& torque);

    // Clear accumulated forces (called after each physics step)
    void ClearForces();
};
