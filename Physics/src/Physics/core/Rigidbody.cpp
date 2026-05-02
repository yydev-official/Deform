#include "Rigidbody.h"
#include <cmath>

void Rigidbody::ApplyForce(const PhysVec3& force)
{
    if (IsKinematic) return;
    ForceAccum += force;
    IsSleeping = false;
    m_sleepTimer = 0.0f;
}

void Rigidbody::ApplyImpulse(const PhysVec3& impulse)
{
    if (IsKinematic) return;
    float invMass = InverseMass();
    if (invMass == 0.0f) return;
    Velocity += impulse * invMass;
    IsSleeping = false;
    m_sleepTimer = 0.0f;
}

void Rigidbody::ApplyCentralForce(const PhysVec3& force)
{
    ApplyForce(force);
}

float Rigidbody::InverseMass() const
{
    return (Mass > 1e-8f) ? 1.0f / Mass : 0.0f;
}

void Rigidbody::Integrate(float dt, const PhysVec3& gravity)
{
    if (IsKinematic || IsSleeping) return;

    // Applica gravità
    if (UseGravity)
        ForceAccum += gravity * Mass;

    // a = F / m
    Acceleration = ForceAccum * InverseMass();

    // Integrazione semi-implicita di Eulero (più stabile del forward Euler)
    Velocity     += Acceleration * dt;
    Velocity     *= std::pow(1.0f - Drag, dt);  // drag esponenziale
    Position     += Velocity * dt;

    // Reset accumulatore forze
    ForceAccum = PhysVec3::Zero();

    // Sleep check
    if (Velocity.LengthSq() < SLEEP_THRESHOLD * SLEEP_THRESHOLD)
    {
        m_sleepTimer += dt;
        if (m_sleepTimer >= SLEEP_TIME_NEEDED)
            IsSleeping = true;
    }
    else
    {
        m_sleepTimer = 0.0f;
    }
}
