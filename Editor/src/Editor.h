#pragma once

#include <Engine.h>

#include "imgui.h"

#include <vector>
#include <string>

namespace Editor {
    struct UIState
    {
        bool showHierarchy = true;
        bool showInspector = true;
        bool showScene = true;
        bool showConsole = true;
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    };

    namespace UI {
        class Hierarchy { public: static void UpdateHierarchy(); };
        class Inspector { public: static void UpdateInspector(); };
        class MainBar { public: static void UpdateMainBar(UIState& state); };
        class Console { public: static void UpdateConsole(); };
    }
}
