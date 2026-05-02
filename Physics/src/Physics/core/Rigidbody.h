#pragma once

#include <Physics/PhysicsAPI.h>
#include <Physics/PhysMath.h>

// ─────────────────────────────────────────────
//  Rigidbody
//  Tiene stato fisico di un oggetto (posizione,
//  velocità, forze, massa). Aggiornato dal
//  PhysicsWorld ogni step.
// ─────────────────────────────────────────────
struct PHYSICS_API Rigidbody
{
    // Stato
    PhysVec3 Position     = PhysVec3::Zero();
    PhysVec3 Velocity     = PhysVec3::Zero();
    PhysVec3 Acceleration = PhysVec3::Zero();

    // Proprietà
    float Mass            = 1.0f;       // kg
    float Drag            = 0.01f;      // resistenza aria (0 = nessuna)
    float Restitution     = 0.4f;       // 0 = anelastico, 1 = perfettamente elastico
    bool  UseGravity      = true;
    bool  IsKinematic     = false;      // se true: non risponde alle forze
    bool  IsSleeping      = false;      // ottimizzazione: oggetti fermi non vengono aggiornati

    // Accumulator forze (resetato ogni step)
    PhysVec3 ForceAccum   = PhysVec3::Zero();

    // API pubblica
    void ApplyForce(const PhysVec3& force);
    void ApplyImpulse(const PhysVec3& impulse);   // velocità istantanea
    void ApplyCentralForce(const PhysVec3& force); // alias chiaro

    float InverseMass() const;

    // Integrate — chiamato da PhysicsWorld
    void Integrate(float dt, const PhysVec3& gravity);

private:
    static constexpr float SLEEP_THRESHOLD  = 0.01f;  // velocità minima prima di dormire
    static constexpr float SLEEP_TIME_NEEDED = 0.5f;   // secondi fermi prima di sleep
    float m_sleepTimer = 0.0f;
};
