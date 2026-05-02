-- collider_component.lua
-- Deform Engine — Collider Component Script
-- Gestisce la forma del collider per il physics engine.
-- Supporta AABB e Sphere. Modificabile dall'utente.

ColliderComponent = {}
ColliderComponent.__index = ColliderComponent

-- Tipi di collider disponibili
ColliderComponent.Type = {
    AABB   = "AABB",
    Sphere = "Sphere"
}

-- Crea un collider AABB (box)
-- entityId: ID dell'entità
-- min: Vector3 {x, y, z} — angolo minimo del box
-- max: Vector3 {x, y, z} — angolo massimo del box
function ColliderComponent.newAABB(entityId, min, max)
    local self = setmetatable({}, ColliderComponent)

    self.entityId   = entityId
    self.type       = ColliderComponent.Type.AABB
    self.isTrigger  = false
    self.enabled    = true

    -- Default: box 1x1x1 centrato sull'origine
    self.aabb = {
        min = min or { x = -0.5, y = -0.5, z = -0.5 },
        max = max or { x =  0.5, y =  0.5, z =  0.5 }
    }

    return self
end

-- Crea un collider Sphere
-- entityId: ID dell'entità
-- radius: raggio della sfera
-- center: Vector3 offset dal centro dell'entità (default 0,0,0)
function ColliderComponent.newSphere(entityId, radius, center)
    local self = setmetatable({}, ColliderComponent)

    self.entityId   = entityId
    self.type       = ColliderComponent.Type.Sphere
    self.isTrigger  = false
    self.enabled    = true

    self.sphere = {
        center = center or { x = 0, y = 0, z = 0 },
        radius = radius or 0.5
    }

    return self
end

-- Imposta come trigger (rileva collisioni ma non risponde)
function ColliderComponent:setTrigger(value)
    self.isTrigger = value
end

-- Scala il collider (es. dopo aver scalato l'entità)
function ColliderComponent:scaleAABB(scaleX, scaleY, scaleZ)
    if self.type ~= ColliderComponent.Type.AABB then return end
    self.aabb.min.x = self.aabb.min.x * scaleX
    self.aabb.min.y = self.aabb.min.y * scaleY
    self.aabb.min.z = self.aabb.min.z * scaleZ
    self.aabb.max.x = self.aabb.max.x * scaleX
    self.aabb.max.y = self.aabb.max.y * scaleY
    self.aabb.max.z = self.aabb.max.z * scaleZ
end

function ColliderComponent:scaleSphere(factor)
    if self.type ~= ColliderComponent.Type.Sphere then return end
    self.sphere.radius = self.sphere.radius * factor
end

-- Callback chiamata quando inizia una collisione/trigger
function ColliderComponent:onTriggerEnter(otherEntityId)
    -- Override nei tuoi script
end

-- Callback chiamata quando finisce una collisione/trigger
function ColliderComponent:onTriggerExit(otherEntityId)
    -- Override nei tuoi script
end

-- Debug: stampa info collider
function ColliderComponent:debugPrint()
    if self.type == ColliderComponent.Type.AABB then
        print(string.format(
            "[Collider AABB] Entity %d | Min(%.2f, %.2f, %.2f) Max(%.2f, %.2f, %.2f)",
            self.entityId,
            self.aabb.min.x, self.aabb.min.y, self.aabb.min.z,
            self.aabb.max.x, self.aabb.max.y, self.aabb.max.z
        ))
    else
        print(string.format(
            "[Collider Sphere] Entity %d | Center(%.2f, %.2f, %.2f) Radius %.2f",
            self.entityId,
            self.sphere.center.x, self.sphere.center.y, self.sphere.center.z,
            self.sphere.radius
        ))
    end
end

return ColliderComponent
