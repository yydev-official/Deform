#pragma once

#include <GLFW/glfw3.h>
#include <EngineAPI.h>

enum class WindowGraphicsAPI
{
    NoAPI,
    OpenGL
};

class DEFORM_API Window {
    public:

        // CONSTRUCTOR AND DESTRUCTOR
        Window();

        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        // FACTORY METHODS
        bool Create(const char* name, int width, int height, WindowGraphicsAPI api = WindowGraphicsAPI::NoAPI);
        void Destroy();
        void PollEvents() const;
        bool ShouldClose() const;

        // GETTER METHODS
        GLFWwindow* GetWindow() const;
        void GetFramebufferSize(int& width, int& height) const;

        int GetHeight() const;
        
        int GetWidth() const;

    private:
        GLFWwindow* m_Window = nullptr;

        int m_Width = 0;

        int m_Height = 0;

        bool m_Created = false;
};
