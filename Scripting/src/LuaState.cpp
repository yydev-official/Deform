#include "LuaState.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <Physics/PhysicsComponent.h>
#include <Engine/core/maths/vector/Vector.h>

#include <iostream>
#include <fstream>

// ─────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────
static Vector3D lua_toVector3(lua_State* L, int idx)
{
    lua_getfield(L, idx, "x"); float x = (float)lua_tonumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, idx, "y"); float y = (float)lua_tonumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, idx, "z"); float z = (float)lua_tonumber(L, -1); lua_pop(L, 1);
    return {x, y, z};
}

static void lua_pushVector3(lua_State* L, const Vector3D& v)
{
    lua_newtable(L);
    lua_pushnumber(L, v.X); lua_setfield(L, -2, "x");
    lua_pushnumber(L, v.Y); lua_setfield(L, -2, "y");
    lua_pushnumber(L, v.Z); lua_setfield(L, -2, "z");
}

// ─────────────────────────────────────────────
//  LuaState
// ─────────────────────────────────────────────
LuaState::LuaState() {}

LuaState::~LuaState()
{
    if (m_L) { lua_close(m_L); m_L = nullptr; }
}

bool LuaState::Initialize(Scene* scene, PhysicsWorld* physicsWorld)
{
    m_scene        = scene;
    m_physicsWorld = physicsWorld;

    m_L = luaL_newstate();
    if (!m_L) return false;

    luaL_openlibs(m_L);

    // Store scene/world pointer accessible from C callbacks
    lua_pushlightuserdata(m_L, this);
    lua_setglobal(m_L, "__deform_state");

    RegisterVector3();
    RegisterEntity();
    RegisterScene();
    RegisterPhysics();
    RegisterTime();

    return true;
}

bool LuaState::CheckLuaError(int result)
{
    if (result != LUA_OK)
    {
        m_lastError = lua_tostring(m_L, -1);
        lua_pop(m_L, 1);
        std::cerr << "[Lua Error] " << m_lastError << "\n";
        return false;
    }
    return true;
}

bool LuaState::ExecuteFile(const std::string& path)
{
    return CheckLuaError(luaL_dofile(m_L, path.c_str()));
}

bool LuaState::ExecuteString(const std::string& code)
{
    return CheckLuaError(luaL_dostring(m_L, code.c_str()));
}

void LuaState::LoadDirectory(const std::filesystem::path& dir)
{
    if (!std::filesystem::exists(dir)) return;
    for (auto& entry : std::filesystem::recursive_directory_iterator(dir))
        if (entry.path().extension() == ".lua")
            ExecuteFile(entry.path().string());
}

bool LuaState::ReloadScript(const std::string& path)
{
    return ExecuteFile(path);
}

// ─────────────────────────────────────────────
//  Binding: Vector3
// ─────────────────────────────────────────────
void LuaState::RegisterVector3()
{
    // Vector3.new(x, y, z)
    lua_newtable(m_L);

    lua_pushcfunction(m_L, [](lua_State* L) -> int {
        float x = (float)luaL_optnumber(L, 1, 0.0);
        float y = (float)luaL_optnumber(L, 2, 0.0);
        float z = (float)luaL_optnumber(L, 3, 0.0);
        lua_pushVector3(L, {x, y, z});
        return 1;
    });
    lua_setfield(m_L, -2, "new");

    lua_pushcfunction(m_L, [](lua_State* L) -> int {
        lua_pushVector3(L, Vector3D::Zero()); return 1;
    });
    lua_setfield(m_L, -2, "zero");

    lua_pushcfunction(m_L, [](lua_State* L) -> int {
        lua_pushVector3(L, Vector3D::Up()); return 1;
    });
    lua_setfield(m_L, -2, "up");

    lua_setglobal(m_L, "Vector3");
}

// ─────────────────────────────────────────────
//  Binding: Entity / Scene
// ─────────────────────────────────────────────
void LuaState::RegisterEntity()
{
    // Entity is accessed via Scene functions, returning integer IDs
    // entity.getPosition(id), entity.setPosition(id, vec3), etc.

    lua_newtable(m_L);

    // entity.getPosition(id) -> Vector3
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self   = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id  = (EntityID)lua_tointeger(L, 1);
        Entity* e    = self->m_scene->GetEntity(id);
        if (!e) { lua_pushnil(L); return 1; }
        auto* tr = e->GetComponent<TransformComponent>();
        if (!tr) { lua_pushnil(L); return 1; }
        lua_pushVector3(L, tr->Position);
        return 1;
    }, 1);
    lua_setfield(m_L, -2, "getPosition");

    // entity.setPosition(id, vec3)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) return 0;
        auto* tr = e->GetComponent<TransformComponent>();
        if (tr) tr->Position = lua_toVector3(L, 2);
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "setPosition");

    // entity.getName(id) -> string
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) { lua_pushnil(L); return 1; }
        lua_pushstring(L, e->GetName().c_str());
        return 1;
    }, 1);
    lua_setfield(m_L, -2, "getName");

    // entity.destroy(id)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        self->m_scene->DestroyEntity(id);
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "destroy");

    lua_setglobal(m_L, "Entity");
}

void LuaState::RegisterScene()
{
    lua_newtable(m_L);

    // Scene.createEntity(name) -> id
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        const char* name = luaL_optstring(L, 1, "Entity");
        EntityID id = self->m_scene->CreateEntity(name);
        lua_pushinteger(L, (lua_Integer)id);
        return 1;
    }, 1);
    lua_setfield(m_L, -2, "createEntity");

    // Scene.getAllEntities() -> table of ids
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self    = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        auto  entities = self->m_scene->GetAllEntities();
        lua_newtable(L);
        for (int i = 0; i < (int)entities.size(); ++i)
        {
            lua_pushinteger(L, (lua_Integer)entities[i]);
            lua_rawseti(L, -2, i + 1);
        }
        return 1;
    }, 1);
    lua_setfield(m_L, -2, "getAllEntities");

    lua_setglobal(m_L, "Scene");
}

// ─────────────────────────────────────────────
//  Binding: Physics
// ─────────────────────────────────────────────
void LuaState::RegisterPhysics()
{
    lua_newtable(m_L);

    // Physics.addForce(entityId, vec3)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Vector3D f  = lua_toVector3(L, 2);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) return 0;
        auto* rb = e->GetComponent<RigidbodyComponent>();
        if (rb) rb->Body.AddForce(f);
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "addForce");

    // Physics.addImpulse(entityId, vec3)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Vector3D v  = lua_toVector3(L, 2);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) return 0;
        auto* rb = e->GetComponent<RigidbodyComponent>();
        if (rb) rb->Body.AddImpulse(v);
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "addImpulse");

    // Physics.getVelocity(entityId) -> vec3
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) { lua_pushnil(L); return 1; }
        auto* rb = e->GetComponent<RigidbodyComponent>();
        if (!rb) { lua_pushnil(L); return 1; }
        lua_pushVector3(L, rb->Body.Velocity);
        return 1;
    }, 1);
    lua_setfield(m_L, -2, "getVelocity");

    // Physics.setVelocity(entityId, vec3)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self  = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        EntityID id = (EntityID)lua_tointeger(L, 1);
        Vector3D v  = lua_toVector3(L, 2);
        Entity* e   = self->m_scene->GetEntity(id);
        if (!e) return 0;
        auto* rb = e->GetComponent<RigidbodyComponent>();
        if (rb) rb->Body.Velocity = v;
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "setVelocity");

    // Physics.setGravity(vec3)
    lua_pushlightuserdata(m_L, this);
    lua_pushcclosure(m_L, [](lua_State* L) -> int {
        auto* self = static_cast<LuaState*>(lua_touserdata(L, lua_upvalueindex(1)));
        self->m_physicsWorld->Gravity = lua_toVector3(L, 1);
        return 0;
    }, 1);
    lua_setfield(m_L, -2, "setGravity");

    lua_setglobal(m_L, "Physics");
}

// ─────────────────────────────────────────────
//  Binding: Time (placeholder — filled by engine)
// ─────────────────────────────────────────────
void LuaState::RegisterTime()
{
    lua_newtable(m_L);
    lua_pushnumber(m_L, 0.0);  lua_setfield(m_L, -2, "deltaTime");
    lua_pushnumber(m_L, 0.0);  lua_setfield(m_L, -2, "elapsed");
    lua_setglobal(m_L, "Time");
}

void LuaState::RegisterInput() {}
