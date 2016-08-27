#pragma once

class Window
{
private:
	int width, height;
	char title[64];
	bool isInitialized = false;

	//Forward Declaring
	class GLFWwindow *winHandle = nullptr;

public:
	bool Init(int A_Width = 800, int A_Height = 600, char *A_Title = "I need a title");
	bool Step();
	bool Term();
};