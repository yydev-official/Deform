#pragma once
#pragma warning(disable: 4005)

#include "imgui.h"
#include <Engine.h>
#include <Engine/core/ecs/ECS.h>
#include <Scripting/LuaState.h>

#include <memory>
#include <vector>
#include <string>
#include <filesystem>

namespace Editor {

    struct AssetsState
    {
        std::filesystem::path rootPath    = "assets";
        std::filesystem::path currentPath = "assets";
        std::string           selectedFile;
        std::string           fileContent;
        std::string           editedContent;
    };

    struct UIState
    {
        bool  showHierarchy = true;
        bool  showInspector = true;
        bool  showScene     = true;
        bool  showConsole   = true;
        bool  showAssets    = true;
        float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

        // ECS
        std::shared_ptr<Scene>     scene          = std::make_shared<Scene>();
        EntityID                   selectedEntity = NULL_ENTITY;

        // Scripting
        std::shared_ptr<LuaState>  luaState;

        // Assets
        AssetsState assets;
    };

    namespace UI {
        class Hierarchy { public: static void UpdateHierarchy(UIState& state); };
        class Inspector { public: static void UpdateInspector(UIState& state); };
        class MainBar   { public: static void UpdateMainBar(UIState& state);   };
        class Console   { public: static void UpdateConsole();                 };
        class Assets    { public: static void UpdateAssets(UIState& state);    };
    }
}
