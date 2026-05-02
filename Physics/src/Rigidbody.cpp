#include "Rigidbody.h"

void Rigidbody::AddForce(const Vector3D& force)
{
    if (IsKinematic) return;
    Force += force;
}

void Rigidbody::AddImpulse(const Vector3D& impulse)
{
    if (IsKinematic) return;
    Velocity += impulse * InverseMass();
}

void Rigidbody::AddTorque(const Vector3D& torque)
{
    if (IsKinematic) return;
    Torque += torque;
}

void Rigidbody::ClearForces()
{
    Force  = Vector3D::Zero();
    Torque = Vector3D::Zero();
}
