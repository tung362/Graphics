#include <iostream>

#include "Window.h"
#include "crenderutils.h"
#include "Gallery.h"

int main()
{
	Window window;
	Gallery gallery;

	window.Init(800, 600, "I got a title :^)");

	Vertex verts[] = { { 1,1,0,1 },{ 1,-1,0,1 },{ -1,-1,0,1 },{ -1,1,0,1 } };
	unsigned tris[] = { 0,1,2, 2,3,0 };

	gallery.MakeObject("quad", verts, 4, tris, 6);

	gallery.LoadObjectOBJ("Sphere", "../res/models/sphere.obj");
	gallery.LoadShader("Simple", "../res/shaders/SimpleVert.txt", "../res/shaders/SimpleFrag.txt");

	float time = 0;

	while (window.Step())
	{
		time += 0.0016667f;
		//Draw(gallery.GetShader("Simple"), gallery.GetObject("Sphere"), time);
		Draw(gallery.GetShader("Simple"), gallery.GetObject("quad"), time);
	}

	gallery.Term();
	window.Term();
	return 0;
}