#include "GLFW\glfw3.h"
#include "Window.h"
#include "Input.h"
#include <cstring>

bool Input::Init(const Window &window)
{
	memset(keys, 0, sizeof(keys));
	winHandle = window.winHandle;

	glfwSetInputMode(winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mouseX = 0;
	mouseY = 0;
	mouseHorz = 0;
	mouseVert = 0;

	return true;
}

bool Input::Step()
{
	for(int i = 0; i < 350; ++i)
	{
		int res = glfwGetKey(winHandle, i);
		if ((keys[i] == UP || keys[i] == RELEASE) && res == GLFW_PRESS) keys[i] = PRESS;
		else if ((keys[i] == DOWN || keys[i] == PRESS) && res == GLFW_RELEASE) keys[i] = RELEASE;
		else if (keys[i] == PRESS) keys[i] = DOWN;
		else if (keys[i] == RELEASE) keys[i] = UP;
	}

	double prevX = mouseX;
	double prevY = mouseY;
	glfwGetCursorPos(winHandle, &mouseX, &mouseY);

	mouseHorz = mouseX - prevX;
	mouseVert = mouseY - prevY;


	return true;
}

bool Input::Term()
{
	return true;
}
