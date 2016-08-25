#include "GLFW\\glfw3.h"
#include "Window.h"
#include <cstring>

bool Window::Init(int A_Width, int A_Height, char * A_Title)
{
	width = A_Width;
	height = A_Height;
	strcpy_s(title, 64, A_Title);

	glfwInit();
	winHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwMakeContextCurrent(winHandle);

	return true;
}

bool Window::Step()
{
	glfwPollEvents();
	glfwSwapBuffers(winHandle);
	return !glfwWindowShouldClose(winHandle);
}

bool Window::Term()
{
	glfwDestroyWindow(winHandle);
	glfwTerminate();
	winHandle = nullptr;
	isInitialized = false;
	return true;
}
