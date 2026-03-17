#pragma once

#include "graphics/Window.h"
#include "core/render/d3d11/DXRender.h"
#include "config.h"
#include "graphics/Window.h"



class Init {
	public:
		bool InitGLFW();
		bool InitD3d11();
		// bool InitGL();
		// bool InitImgui();
};
