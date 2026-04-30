#pragma once
#pragma warning(disable: 4005)

#include "imgui.h"
#include <Engine.h>
#include <Engine/core/ecs/ECS.h>

#include <memory>
#include <vector>
#include <string>

namespace Editor {

    struct UIState
    {
        bool  showHierarchy = true;
        bool  showInspector = true;
        bool  showScene     = true;
        bool  showConsole   = true;
        float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

        // ECS
        std::shared_ptr<Scene> scene          = std::make_shared<Scene>();
        EntityID               selectedEntity = NULL_ENTITY;
    };

    namespace UI {
        class Hierarchy { public: static void UpdateHierarchy(UIState& state); };
        class Inspector { public: static void UpdateInspector(UIState& state); };
        class MainBar   { public: static void UpdateMainBar(UIState& state);   };
        class Console   { public: static void UpdateConsole();                 };
    }
}
