#pragma once

#ifdef _WIN32
    #ifdef DEFORM_ENGINE_EXPORTS
        #define DEFORM_API __declspec(dllexport)
    #else
        #define DEFORM_API __declspec(dllimport)
    #endif
#else
    // Linux/macOS — GCC visibility
    #ifdef DEFORM_ENGINE_EXPORTS
        #define DEFORM_API __attribute__((visibility("default")))
    #else
        #define DEFORM_API
    #endif
#endif
