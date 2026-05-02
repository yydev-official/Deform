#pragma once

#include "ScriptingAPI.h"
#include <Engine/core/ecs/ECS.h>
#include <Physics/PhysicsWorld.h>

#include <string>
#include <filesystem>

// Forward declare lua_State to avoid including lua.h in headers
struct lua_State;

class SCRIPTING_API LuaState
{
public:
    LuaState();
    ~LuaState();

    // Initialize Lua and register all Deform bindings
    bool Initialize(Scene* scene, PhysicsWorld* physicsWorld);

    // Execute a Lua script file
    bool ExecuteFile(const std::string& path);

    // Execute a Lua string
    bool ExecuteString(const std::string& code);

    // Load all .lua scripts from a directory
    void LoadDirectory(const std::filesystem::path& dir);

    // Reload a specific script
    bool ReloadScript(const std::string& path);

    // Get last error message
    const std::string& GetLastError() const { return m_lastError; }

    // Access raw Lua state (for advanced use)
    lua_State* GetLuaState() { return m_L; }

private:
    lua_State*    m_L           = nullptr;
    Scene*        m_scene       = nullptr;
    PhysicsWorld* m_physicsWorld = nullptr;
    std::string   m_lastError;

    // Register bindings
    void RegisterVector3();
    void RegisterEntity();
    void RegisterScene();
    void RegisterPhysics();
    void RegisterInput();
    void RegisterTime();

    bool CheckLuaError(int result);
};
