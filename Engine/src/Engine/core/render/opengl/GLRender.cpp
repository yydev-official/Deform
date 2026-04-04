#include <GLFW/glfw3.h>

#include "GLRender.h"
#include "../../../../Engine.h"

using namespace deform;

bool GLRender::Initialize(GLFWwindow* window)
{
    if (!window)
    {
        Logger::FatalError("GLRender::Initialize received a null window.");
        return false;
    }

    m_window = window;
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    Logger::Log("OpenGL initialization successful.");
    return true;
}

void GLRender::Shutdown()
{
    if (glfwGetCurrentContext() == m_window)
    {
        glfwMakeContextCurrent(nullptr);
    }

    m_window = nullptr;
}

void GLRender::BeginFrame(const float clearColor[4]) const
{
    if (!m_window)
        return;

    glfwMakeContextCurrent(m_window);

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRender::Present() const
{
    if (m_window)
    {
        glfwSwapBuffers(m_window);
    }
}

bool GLRender::IsInitialized() const
{
    return m_window != nullptr;
}
