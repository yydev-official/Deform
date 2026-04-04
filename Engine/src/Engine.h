#pragma once

//IMPORTANT : ENGINE API HEADER
#include <EngineAPI.h>


#include <vector>
#include <string>

#include "Engine/core/render/d3d11/DXRender.h"
#include "Engine/core/render/opengl/GLRender.h"
#include "Engine/core/render/vulkan/VKRender.h"

#include "Engine/graphics/Window.h"



namespace deform
{
	class Logger {
	public:
		static DEFORM_API void Log(const std::string& msg);
		static DEFORM_API void FatalError(const char* message);

		static DEFORM_API const std::vector<std::string>& GetLogs();

	private:
		static std::vector<std::string> s_Logs;
	};
}
