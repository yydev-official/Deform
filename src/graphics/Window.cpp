//libraries
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
//header files
#include "Window.h"

// CONSTRUCTOR AND OVERLOADS
Window::Window() : m_Window(nullptr), m_Width(0), m_Height(0), m_Created(false) {}

// FACTORY METHODS
bool Window::Create(int width, int height)
{
    if (m_Created)
        return false;

    m_Width = width;
    m_Height = height;

    m_Window = glfwCreateWindow(width, height, "Deform Engine - EDITOR (DirectX 11)", nullptr, nullptr);
    m_Created = (m_Window != nullptr);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //we will apply api manually

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