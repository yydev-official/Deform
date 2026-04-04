//libraries
#include <GLFW/glfw3.h>
//header files
#include "Window.h"

// CONSTRUCTOR AND OVERLOADS
Window::Window() : m_Window(nullptr), m_Width(0), m_Height(0), m_Created(false) {}

// FACTORY METHODS
bool Window::Create(const char* name, int width, int height, WindowGraphicsAPI api)
{
    if (m_Created)
        return false;

    m_Width = width;
    m_Height = height;

    if (!glfwInit())
        return false;

    if (api == WindowGraphicsAPI::OpenGL)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    }
    else
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    m_Window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    m_Created = (m_Window != nullptr);

    return m_Created;

}

void Window::Destroy()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    m_Created = false;
    m_Width = 0;
    m_Height = 0;
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return m_Window == nullptr || glfwWindowShouldClose(m_Window);
}

Window::~Window()
{
    Destroy();
}

// Getter for m_Window

// GETTER METHODS
GLFWwindow* Window::GetWindow() const
{
    return m_Window;
}

void Window::GetFramebufferSize(int& width, int& height) const
{
    width = 0;
    height = 0;

    if (m_Window)
    {
        glfwGetFramebufferSize(m_Window, &width, &height);
    }
}

int Window::GetWidth() const
{
    return m_Width;
}

int Window::GetHeight() const
{
    return m_Height;
}
