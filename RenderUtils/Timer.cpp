#include "GLFW\glfw3.h"
#include "Timer.h"

bool Time::Init()
{
	glfwSetTime(0);
	previousTime = 0;
	currentTime = 0;
	deltaTime = 0;

	return true;
}

bool Time::Step()
{
	previousTime = currentTime;
	currentTime = GetRealTime();
	deltaTime = currentTime - previousTime;

	return true;
}

bool Time::Term()
{
	return true;
}

float Time::GetDeltaTime() const
{
	return deltaTime;
}

float Time::GetTotalTime() const
{
	return currentTime;
}

float Time::GetRealTime() const
{
	return glfwGetTime();
}