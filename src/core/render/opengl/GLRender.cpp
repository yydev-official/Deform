#include <GLFW/glfw3.h>
#include <render/opengl/GLRender.h>


bool GLRender::Initgl(GLFWwindow* hwnd) {
	int loaded = gladLoadGL(glfwGetProcAddress);

	if (!loaded) {
		return false;
	}

	return true;
}

