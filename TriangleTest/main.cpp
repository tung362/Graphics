#include <iostream>

#include "Window.h"
#include "crenderutils.h"
#include "Gallery.h"

int main()
{
	Window window;
	Gallery gallery;

	window.Init(800, 600, "I got a title :^)");

	gallery.LoadObjectOBJ("Sphere", "../res/models/sphere.obj");
	gallery.LoadShader("Simple", "../res/shaders/SimpleVert.txt", "../res/shaders/SimpleFrag.txt");

	float time = 0;

	while (window.Step())
	{
		time += 0.0016667f;
		Draw(gallery.GetShader("Simple"), gallery.GetObject("Sphere"), time);
	}

	gallery.Term();
	window.Term();
	return 0;
}