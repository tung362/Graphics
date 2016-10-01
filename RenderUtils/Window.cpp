#include "gldecs.h"

#include "Window.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

#ifdef _DEBUG
void APIENTRY GL_errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	fprintf(stderr, "%s\n", message);

	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		//system("pause");
	}
}
#endif

bool Window::Init(int A_Width, int A_Height, char * A_Title)
{
	width = A_Width;
	height = A_Height;
	strcpy_s(title, 64, A_Title);

	glfwInit();

	winHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwMakeContextCurrent(winHandle);
	isInitialized = true;

	glewExperimental = true;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(GL_errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif

	return true;
}


bool Window::Step()
{
	if (!isInitialized) return false;

	glfwPollEvents();
	glfwSwapBuffers(winHandle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
