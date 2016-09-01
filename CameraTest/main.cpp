#include <iostream>

#include "Window.h"
#include "crenderutils.h"
#include "Gallery.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

int main()
{
	Window window;
	Gallery gallery;

	window.Init(800, 600, "I got a title :^)");

	gallery.LoadObjectOBJ("Sphere", "../res/models/sphere.obj");
	gallery.LoadObjectOBJ("Cube", "../res/models/cube.obj");
	gallery.LoadObjectOBJ("Crystal", "../res/models/crystal.obj");
	gallery.LoadShader("Simple", "../res/shaders/SimpleVert.txt", "../res/shaders/SimpleFrag.txt");
	gallery.LoadShader("SimpleCamera", "../res/shaders/CameraVert.txt", "../res/shaders/CameraFrag.txt");

	glm::mat4 proj, view, model, model2, model3;
	//proj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	proj = glm::perspective(45.f, 1.f, 0.1f, 100.f);

	view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f),
					   glm::vec3(0.f, 0.f, 0.f),
					   glm::vec3(0.f, 1.f, 0.f));

	//model = glm::scale(glm::vec3( .5f, .5f, .5f )) * glm::translate(glm::vec3(.1f, .1f, .1f));
	model2 = glm::translate(glm::vec3(1, 0, 1));

	float time = 0;

	while (window.Step())
	{
		time += 0.016667f;

		model3 = glm::translate(glm::vec3(3, 0, 3)) * glm::rotate(time, glm::vec3(0, 1, 0));

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Cube"), 
												glm::value_ptr(model), 
												glm::value_ptr(view), 
												glm::value_ptr(proj));

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Sphere"),
												glm::value_ptr(model2),
												glm::value_ptr(view),
												glm::value_ptr(proj));

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Crystal"),
												glm::value_ptr(model3),
												glm::value_ptr(view),
												glm::value_ptr(proj));
	}

	gallery.Term();
	window.Term();
	return 0;
}