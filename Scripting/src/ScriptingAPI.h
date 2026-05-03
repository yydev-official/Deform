#pragma once

#ifdef _WIN32
    #ifdef SCRIPTING_EXPORTS
        #define SCRIPTING_API __declspec(dllexport)
    #else
        #define SCRIPTING_API __declspec(dllimport)
    #endif
#else
    #ifdef SCRIPTING_EXPORTS
        #define SCRIPTING_API __attribute__((visibility("default")))
    #else
        #define SCRIPTING_API
    #endif
#endif