#pragma once

#ifdef DEFORM_ENGINE_EXPORTS
	#define DEFORM_API __declspec(dllexport)
#else
	#define DEFORM_API __declspec(dllimport)
#endif
