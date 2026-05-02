#include "PhysicsWorld.h"
#include <cmath>
#include <algorithm>

// ─────────────────────────────────────────────
//  ColliderComponent helpers
// ─────────────────────────────────────────────
AABBCollider ColliderComponent::GetWorldAABB(const Vector3D& position) const
{
    return AABB.Translated(position);
}

SphereCollider ColliderComponent::GetWorldSphere(const Vector3D& position) const
{
    return Sphere.Translated(position);
}

// ─────────────────────────────────────────────
//  PhysicsWorld
// ─────────────────────────────────────────────
PhysicsWorld::PhysicsWorld() {}

void PhysicsWorld::OnCollision(CollisionCallback callback)
{
    m_callbacks.push_back(std::move(callback));
}

void PhysicsWorld::ClearCallbacks()
{
    m_callbacks.clear();
}

void PhysicsWorld::FireCollisionEvent(EntityID a, EntityID b, const CollisionResult& result)
{
    CollisionEvent evt { a, b, result };
    for (auto& cb : m_callbacks) cb(evt);
}

void PhysicsWorld::Step(Scene& scene, float deltaTime)
{
    float dt = deltaTime / static_cast<float>(SubSteps);
    for (int i = 0; i < SubSteps; ++i)
    {
        IntegrateVelocities(scene, dt);
        DetectAndResolveCollisions(scene);
    }
}

void PhysicsWorld::IntegrateVelocities(Scene& scene, float dt)
{
    auto entities = scene.GetEntitiesWith<RigidbodyComponent>();

    for (EntityID id : entities)
    {
        Entity* entity = scene.GetEntity(id);
        if (!entity || !entity->Active) continue;

        auto* rb = entity->GetComponent<RigidbodyComponent>();
        auto* tr = entity->GetComponent<TransformComponent>();
        if (!rb || !tr) continue;

        Rigidbody& body = rb->Body;
        if (body.IsKinematic) continue;

        // Apply gravity
        if (body.UseGravity)
            body.AddForce(Gravity * body.Mass);

        // F = ma → a = F/m
        body.Acceleration = body.Force * body.InverseMass();

        // Integrate velocity
        body.Velocity += body.Acceleration * dt;

        // Apply linear drag
        body.Velocity *= (1.0f - std::min(body.Drag * dt, 1.0f));

        // Integrate angular velocity
        body.AngularVelocity += body.Torque * body.InverseMass() * dt;
        body.AngularVelocity *= (1.0f - std::min(body.AngularDrag * dt, 1.0f));

        // Freeze axes if requested
        if (body.FreezePositionX) body.Velocity.X = 0.0f;
        if (body.FreezePositionY) body.Velocity.Y = 0.0f;
        if (body.FreezePositionZ) body.Velocity.Z = 0.0f;

        // Integrate position
        tr->Position += body.Velocity * dt;

        // Integrate rotation (simple Euler)
        if (body.AngularVelocity.LengthSquared() > 1e-8f)
        {
            float angle = body.AngularVelocity.Length() * dt;
            Vector3D axis = body.AngularVelocity.Normalized();
            Quaternion deltaRot = Quaternion::FromAxisAngle(axis, angle);
            tr->Rotation = (deltaRot * tr->Rotation).Normalized();
        }

        body.ClearForces();
    }
}

void PhysicsWorld::DetectAndResolveCollisions(Scene& scene)
{
    auto entities = scene.GetEntitiesWith<ColliderComponent>();

    for (size_t i = 0; i < entities.size(); ++i)
    {
        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            EntityID idA = entities[i];
            EntityID idB = entities[j];

            Entity* a = scene.GetEntity(idA);
            Entity* b = scene.GetEntity(idB);
            if (!a || !b || !a->Active || !b->Active) continue;

            auto* colA = a->GetComponent<ColliderComponent>();
            auto* colB = b->GetComponent<ColliderComponent>();
            auto* trA  = a->GetComponent<TransformComponent>();
            auto* trB  = b->GetComponent<TransformComponent>();
            if (!colA || !colB || !trA || !trB) continue;

            // Test collision
            CollisionResult result;

            if (colA->Type == ColliderType::AABB && colB->Type == ColliderType::AABB)
                result = TestAABBvsAABB(colA->GetWorldAABB(trA->Position),
                                        colB->GetWorldAABB(trB->Position));

            else if (colA->Type == ColliderType::Sphere && colB->Type == ColliderType::Sphere)
                result = TestSphereVsSphere(colA->GetWorldSphere(trA->Position),
                                            colB->GetWorldSphere(trB->Position));

            else if (colA->Type == ColliderType::AABB && colB->Type == ColliderType::Sphere)
                result = TestAABBvsSphere(colA->GetWorldAABB(trA->Position),
                                          colB->GetWorldSphere(trB->Position));

            else if (colA->Type == ColliderType::Sphere && colB->Type == ColliderType::AABB)
            {
                result = TestAABBvsSphere(colB->GetWorldAABB(trB->Position),
                                          colA->GetWorldSphere(trA->Position));
                result.Normal = -result.Normal;
            }

            if (!result.Hit) continue;

            FireCollisionEvent(idA, idB, result);

            // Skip resolution for triggers
            if (colA->IsTrigger || colB->IsTrigger) continue;

            ResolveCollision(*a, *b, result);
        }
    }
}

void PhysicsWorld::ResolveCollision(Entity& a, Entity& b, const CollisionResult& result)
{
    auto* rbA = a.GetComponent<RigidbodyComponent>();
    auto* rbB = b.GetComponent<RigidbodyComponent>();
    auto* trA = a.GetComponent<TransformComponent>();
    auto* trB = b.GetComponent<TransformComponent>();
    if (!trA || !trB) return;

    bool kinA = !rbA || rbA->Body.IsKinematic;
    bool kinB = !rbB || rbB->Body.IsKinematic;

    float invMassA = kinA ? 0.0f : rbA->Body.InverseMass();
    float invMassB = kinB ? 0.0f : rbB->Body.InverseMass();
    float totalInvMass = invMassA + invMassB;

    if (totalInvMass < 1e-8f) return; // both infinite mass

    // ── Positional correction (prevent sinking) ──────────────────
    const float slop    = 0.01f;  // small penetration allowed
    const float percent = 0.8f;   // correction factor

    float correctionMag = std::max(result.PenetrationDepth - slop, 0.0f)
                        / totalInvMass * percent;
    Vector3D correction = result.Normal * correctionMag;

    if (!kinA) trA->Position -= correction * invMassA;
    if (!kinB) trB->Position += correction * invMassB;

    // ── Impulse resolution ───────────────────────────────────────
    Vector3D velA = rbA ? rbA->Body.Velocity : Vector3D::Zero();
    Vector3D velB = rbB ? rbB->Body.Velocity : Vector3D::Zero();
    Vector3D relVel = velB - velA;

    float velAlongNormal = relVel.Dot(result.Normal);
    if (velAlongNormal > 0) return; // separating, no impulse needed

    float restitution = 0.3f;
    if (rbA) restitution = std::min(restitution, rbA->Body.Restitution);
    if (rbB) restitution = std::min(restitution, rbB->Body.Restitution);

    float impulseMag = -(1.0f + restitution) * velAlongNormal / totalInvMass;
    Vector3D impulse = result.Normal * impulseMag;

    if (rbA && !kinA) rbA->Body.Velocity -= impulse * invMassA;
    if (rbB && !kinB) rbB->Body.Velocity += impulse * invMassB;

    // ── Friction ─────────────────────────────────────────────────
    relVel = (rbB ? rbB->Body.Velocity : Vector3D::Zero())
           - (rbA ? rbA->Body.Velocity : Vector3D::Zero());

    Vector3D tangent = relVel - result.Normal * relVel.Dot(result.Normal);
    if (tangent.LengthSquared() < 1e-8f) return;
    tangent.Normalize();

    float frictionMag = -relVel.Dot(tangent) / totalInvMass;

    float friction = 0.5f;
    if (rbA) friction = (friction + rbA->Body.Friction) * 0.5f;
    if (rbB) friction = (friction + rbB->Body.Friction) * 0.5f;

    float clampedFriction = std::abs(frictionMag) < std::abs(impulseMag) * friction
        ? frictionMag
        : -impulseMag * friction;

    Vector3D frictionImpulse = tangent * clampedFriction;
    if (rbA && !kinA) rbA->Body.Velocity -= frictionImpulse * invMassA;
    if (rbB && !kinB) rbB->Body.Velocity += frictionImpulse * invMassB;
}
