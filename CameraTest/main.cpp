#include <iostream>

#include "Window.h"
#include "crenderutils.h"
#include "Gallery.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

int main()
{
	Window window;
	Gallery gallery;
	Time time;
	Input input;

	window.Init(1280, 720, "I got a title :^)");
	gallery.Init();
	input.Init(window);
	time.Init();

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

	float dt = 0;
	float ct = 0;
	float colorswitch = 0;

	FlyCamera cam;
	cam.JumpTo(glm::vec3(20, 0, 0));
	cam.LookAt(glm::vec3(0, 0, 0));

	while (window.Step())
	{
		time.Step();
		input.Step();
		dt = time.GetDeltaTime();

		view = cam.GetView();
		proj = cam.GetProjection();

		cam.Update(input, time);

		if (input.GetKeyState('D') == Input::DOWN) ct += time.GetDeltaTime();
		if (input.GetKeyState('A') == Input::DOWN) ct -= time.GetDeltaTime();

		if (input.GetKeyState('1') == Input::PRESS) colorswitch = 0;
		if (input.GetKeyState('2') == Input::PRESS) colorswitch = 1;

		model3 = glm::translate(glm::vec3(3, 0, 3)) * glm::rotate(ct, glm::vec3(0, 1, 0));

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Cube"), 
												glm::value_ptr(model), 
												glm::value_ptr(view), 
												glm::value_ptr(proj), colorswitch);

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Sphere"),
												glm::value_ptr(model2),
												glm::value_ptr(view),
												glm::value_ptr(proj), colorswitch);

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Crystal"),
												glm::value_ptr(model3),
												glm::value_ptr(view),
												glm::value_ptr(proj), colorswitch);

	}

	gallery.Term();
	input.Term();
	time.Term();
	window.Term();
	return 0;
}