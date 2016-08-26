#include "Window.h"
#include "crenderutils.h"
#include "Vertex.h"
#include <iostream>

int main()
{
	Window window;
	window.Init();

	Geometry geometry2 = LoadObj("../res/models/sphere.obj");
	Shader shader = LoadShader("../res/shaders/SimpleVert.txt", "../res/shaders/SimpleFrag.txt");

	while (window.Step())
	{
		Draw(shader, geometry2);
	}

	FreeGeometry(geometry2);
	FreeShader(shader);
	window.Term();
	return 0;
}