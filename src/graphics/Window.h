#pragma once
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Window {
    public:

        // CONSTRUCTOR AND DESTRUCTOR
        Window();

        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        // FACTORY METHODS
        bool Create(int width, int height);

        // GETTER METHODS
        GLFWwindow* GetWindow() const;

        int GetHeight() const;
        
        int GetWidth() const;

    private:
        GLFWwindow* m_Window = nullptr;

        int m_Width = 0;

        int m_Height = 0;

        bool m_Created = false;
};