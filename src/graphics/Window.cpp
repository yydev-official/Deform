// Window.cpp

//libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//header files
#include "Window.h"

// CONSTRUCTOR AND OVERLOADS
Window::Window() : m_Window(nullptr), m_Width(0), m_Height(0), m_Created(false)
{
}

// FACTORY METHODS

bool Window::Create(int width, int height)
{
    if (m_Created)
        return false;

    m_Width = width;
    m_Height = height;

    m_Window = glfwCreateWindow(width, height, "Trail Engine", nullptr, nullptr);
    m_Created = (m_Window != nullptr);

    return m_Created;

}

Window::~Window()
{
    if (m_Window)
        glfwDestroyWindow(m_Window);
}

// Getter for m_Window

// GETTER METHODS
GLFWwindow* Window::GetWindow() const
{
    return m_Window; // I'll fix it up. -- Tradic // I FIXED IT, we were returning pointer but return type was GLFWwindow
}

int Window::GetWidth() const
{
    return m_Width;
}

int Window::GetHeight() const
{
    return m_Height;
}