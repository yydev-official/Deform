#include "GLFW/include/GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"

void InitImgui(GLFWwindow* Window) {

	ImGui::CreateContext();
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("imgui window");
	ImGui::Text("Hello, world!");
	ImGui::End();

}


int main()
{
	glfwInit();

	GLFWwindow* Window = glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr);
	glfwMakeContextCurrent(Window);

	ImGui_ImplGlfw_InitForOther(Window, true);

	while (!glfwWindowShouldClose(glfwGetCurrentContext()))
	{
		glfwPollEvents();
		InitImgui(Window);
	}

	return 0;
}