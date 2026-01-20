//main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics\Window.h"

int main(void)
{

    if (!glfwInit())
        return -1;

    {
        Window window;

        // Stop if can't create
        if(!window.Create(1280, 720))
        {
            glfwTerminate();
            return -1;
        }

        if (!window.GetWindow()) 
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window.GetWindow());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            return -1;
        }

        while (!glfwWindowShouldClose(window.GetWindow()))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window.GetWindow());
            glfwPollEvents();

        }
    }

    glfwTerminate();
    return 0;

}