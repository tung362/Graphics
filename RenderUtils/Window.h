#pragma once

class Window
{
private:
	friend class Input;
	int width, height;
	char title[64];
	bool isInitialized = false;

	//Forward Declaring
	struct GLFWwindow *winHandle = nullptr;

public:
	bool Init(int A_Width = 800, int A_Height = 600, char *A_Title = "I need a title");
	bool Step();
	bool Term();
};