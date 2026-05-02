-- physics_component.lua
-- Deform Engine — Physics Component Script
-- Modifica questo file per cambiare il comportamento fisico di default.
-- Puoi anche crearne una copia con un nome diverso e assegnarla a un'entità specifica.

PhysicsComponent = {}
PhysicsComponent.__index = PhysicsComponent

-- Crea un nuovo PhysicsComponent per un'entità
-- entityId: ID dell'entità ECS
-- config: tabella opzionale con proprietà iniziali
function PhysicsComponent.new(entityId, config)
    local self = setmetatable({}, PhysicsComponent)

    self.entityId = entityId
    self.enabled  = true

    -- Proprietà di default (sovrascrivibili via config)
    self.mass        = (config and config.mass)        or 1.0
    self.drag        = (config and config.drag)        or 0.01
    self.useGravity  = (config and config.useGravity)  ~= false  -- default true
    self.isKinematic = (config and config.isKinematic) or false
    self.restitution = (config and config.restitution) or 0.3
    self.friction    = (config and config.friction)    or 0.5

    return self
end

-- Applica una forza all'entità
function PhysicsComponent:addForce(x, y, z)
    if not self.enabled or self.isKinematic then return end
    Physics.addForce(self.entityId, Vector3.new(x, y, z))
end

-- Applica un impulso immediato
function PhysicsComponent:addImpulse(x, y, z)
    if not self.enabled or self.isKinematic then return end
    Physics.addImpulse(self.entityId, Vector3.new(x, y, z))
end

-- Ottieni la velocità attuale
function PhysicsComponent:getVelocity()
    return Physics.getVelocity(self.entityId)
end

-- Imposta la velocità direttamente
function PhysicsComponent:setVelocity(x, y, z)
    Physics.setVelocity(self.entityId, Vector3.new(x, y, z))
end

-- Ferma il movimento
function PhysicsComponent:stop()
    self:setVelocity(0, 0, 0)
end

-- Sposta verso l'alto (es. salto)
function PhysicsComponent:jump(force)
    force = force or 5.0
    self:addImpulse(0, force, 0)
end

-- Callback chiamata ogni frame (Override nei tuoi script)
function PhysicsComponent:onUpdate()
    -- Override in script personalizzati
end

-- Callback chiamata alla collisione
-- otherEntityId: ID dell'altra entità
function PhysicsComponent:onCollision(otherEntityId)
    -- Override in script personalizzati
    -- Esempio:
    -- print("Collisione con entità " .. tostring(otherEntityId))
end

return PhysicsComponent
