#pragma once

#ifdef DEFORM_PHYSICS_EXPORTS
    #define PHYSICS_API __declspec(dllexport)
#else
    #define PHYSICS_API __declspec(dllimport)
#endif
