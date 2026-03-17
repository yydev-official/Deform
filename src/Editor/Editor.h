#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "../core/render/d3d11/DXRender.h"

#include <vector>
#include <string>
#include <Windows.h>


namespace Editor {

	class UI {
		public:
			bool Init();
			void Update();
	};

	class Logger {
		public:
			static void Log(const std::string& msg);
			static void FatalError(const char* message);

			static const std::vector<std::string>& GetLogs();

		private:
			static std::vector<std::string> s_Logs;
	};

}