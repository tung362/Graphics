#include "crenderutils.h"
#include "RadialGradientGen.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "Procgen.h"
#include "GLM\ext.hpp"
#include <iostream>

/*
------------------------------------
CONTROLS:
Move camera: wasd
Look around: move mouse
-: increase value
=: decrease value
1: Change value type to saturation
2: Change value type to brightness
3: Change value type to contrast
4: Change value type to mountainScale
5: Change value type to mountainX
6: Change value type to mountainY
9: Change illumination model
------------------------------------
*/

void main()
{
	Window context;
	Input input;
	Time time;

	context.Init(1280, 720);
	input.Init(context);
	time.Init();

	//Objects//
	Geometry quad = MakeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = LoadObj("../res/models/soulspear.obj");
	Geometry sphere = LoadObj("../res/models/sphere.obj");
	Geometry plane = GenGrid(512, 20);
	Geometry plane2 = GenGrid(512, 20);

	//Generated objects
	//Texture noise = GenRadialGradient(64, glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), 0.5f);


	//Load objs//
	Texture spear_normal = LoadTex("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = LoadTex("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = LoadTex("../res/textures/soulspear_specular.tga");
	Texture heighttex = LoadTex("../res/textures/height.jpg");
	Texture meleetex = LoadTex("../res/textures/melee.jpg");

	//Texture combine
	Texture spear_Textures[3] = { spear_normal, spear_diffuse, spear_specular };


	//Custom color texture//
	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = MakeTex(1, 1, 4, norm_pixels);
	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = MakeTex(1, 1, 4, white_pixels);
	const unsigned char red_pixels[4] = { 255, 0, 0, 255 };
	Texture red = MakeTex(1, 1, 4, red_pixels);
	const unsigned char blue_pixels[4] = { 0, 0, 255, 255 };
	Texture blue = MakeTex(1, 1, 4, blue_pixels);


	//Shaders//
	Shader qdraw = LoadShader("../res/shaders/Quad.vert", "../res/shaders/Quad.frag", false);
	Shader gpass = LoadShader("../res/shaders/Gpass.vert", "../res/shaders/Gpass.frag");
	//Light shader
	Shader spass = LoadShader("../res/shaders/Spass.vert", "../res/shaders/Spass.frag", true, false, false);
	Shader lpass = LoadShader("../res/shaders/Lspass.vert", "../res/shaders/Lspass.frag", false, true);
	//Post shader
	Shader postShader = LoadShader("../res/shaders/Post.vert", "../res/shaders/Post.frag");


	//Frame buffers//
	bool isFTex[] = { false, true, false, true }; // colors don't need floats, but position/normal should use it.
	Framebuffer screen = { 0, 1280, 720 };
	Framebuffer geometryFrame = MakeFramebuffer(1280, 720, 4, isFTex);
	Framebuffer lightFrame = MakeFramebuffer(1280, 720, 3);
	//Shadow res
	Framebuffer shadowFrame = MakeFramebuffer(1024, 1024, 0);
	//Post
	Framebuffer postFrame = MakeFramebuffer(1280, 720, 3);



	//Camera information//
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 camProj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);


	//Model matrix//
	glm::mat4 spearModel;
	glm::mat4 sphereModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));
	glm::mat4 wallModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) * glm::translate(glm::vec3(0, 0, -2)) * glm::scale(glm::vec3(2, 2, 1));
	glm::mat4 planeModel = glm::translate(glm::vec3(0, -2, 0));
	glm::mat4 planeModel2 = glm::translate(glm::vec3(-20, -2, 0));
	glm::mat4 meleeModel = glm::translate(glm::vec3(-40, -2, 0));


	//Light matrix//
	glm::mat4 lightProj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 redView = glm::lookAt(glm::normalize(-glm::vec3(1, -1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 redColor = glm::vec4(1, 0, 0, 1);
	glm::mat4 greenView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 greenColor = glm::vec4(0, 1, 0, 1);
	glm::mat4 blueView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 blueColor = glm::vec4(0, 0, 1, 1);
	glm::mat4 whiteView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);

	//Camera
	FlyCamera cam;
	cam.JumpTo(glm::vec3(0, 0, -10));
	//cam.LookAt(glm::vec3(0, 0, 0));


	//Post process settings
	int changeType = 1; //1 = saturation, 2 = brightness, 3 = contrast
	float saturation = 0.5f;
	float brightness = 1;
	float contrast = 1;
	float mountainScale = 1;
	float mountainX = 0.5f;
	float mountainY = 0.5f;
	bool isPhong = true;

	while (context.Step())
	{
		time.Step();
		input.Step();

		//Value inputs
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if (input.GetKeyState('1') == Input::RELEASE) changeType = 1;
		if (input.GetKeyState('2') == Input::RELEASE) changeType = 2;
		if (input.GetKeyState('3') == Input::RELEASE) changeType = 3;
		if (input.GetKeyState('4') == Input::RELEASE) changeType = 4;
		if (input.GetKeyState('5') == Input::RELEASE) changeType = 5;
		if (input.GetKeyState('6') == Input::RELEASE) changeType = 6;

		if (input.GetKeyState('9') == Input::RELEASE)
		{
			if (isPhong) isPhong = false;
			else isPhong = true;
			std::cout << isPhong << std::endl;
		}

		//Just for realtime changes
		switch (changeType)
		{
		case 1:
			if (input.GetKeyState('-') == Input::DOWN) saturation -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) saturation += 2 * time.GetDeltaTime();
			break;
		case 2:
			if (input.GetKeyState('-') == Input::DOWN) brightness -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) brightness += 2 * time.GetDeltaTime();
			break;
		case 3:
			if (input.GetKeyState('-') == Input::DOWN) contrast -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) contrast += 2 * time.GetDeltaTime();
			break;
		case 4:
			if (input.GetKeyState('-') == Input::DOWN) mountainScale -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) mountainScale += 2 * time.GetDeltaTime();
			break;
		case 5:
			if (input.GetKeyState('-') == Input::DOWN) mountainX -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) mountainX += 2 * time.GetDeltaTime();
			break;
		case 6:
			if (input.GetKeyState('-') == Input::DOWN) mountainY -= 2 * time.GetDeltaTime();
			if (input.GetKeyState('=') == Input::DOWN) mountainY += 2 * time.GetDeltaTime();
			break;
		default:
			break;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		spearModel = glm::rotate(time.GetTotalTime(), glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(1, 1, 1));

		camView = cam.GetView();
		camProj = cam.GetProjection();
		cam.Update(input, time);

		//Geometry pass//
		ClearFramebuffer(geometryFrame);
		Tdraw(gpass, spear, geometryFrame, spearModel, camView, camProj, spear_diffuse, spear_normal, spear_specular, 0);
		Tdraw(gpass, sphere, geometryFrame, sphereModel, camView, camProj, white, vertex_normals, white, 0);
		Tdraw(gpass, quad, geometryFrame, wallModel, camView, camProj, white, vertex_normals, white, 0);
		Tdraw(gpass, plane, geometryFrame, planeModel, camView, camProj, white, vertex_normals, white, 1, mountainScale, mountainX, mountainY);
		Tdraw(gpass, plane2, geometryFrame, planeModel2, camView, camProj, heighttex, vertex_normals, white, 2, mountainScale, mountainX, mountainY, heighttex);
		Tdraw(gpass, plane2, geometryFrame, meleeModel, camView, camProj, meleetex, vertex_normals, white, 2, mountainScale, mountainX, mountainY, meleetex);


		//Light pass//
		ClearFramebuffer(lightFrame);
		//Shadow prePass
		//RED LIGHT
		ClearFramebuffer(shadowFrame);
		Tdraw(spass, spear, shadowFrame, spearModel, redView, lightProj);
		Tdraw(spass, sphere, shadowFrame, sphereModel, redView, lightProj);
		Tdraw(spass, quad, shadowFrame, wallModel, redView, lightProj);
		//Tdraw(spass, plane, shadowFrame, planeModel, redView, lightProj);
		//Tdraw(spass, plane2, shadowFrame, planeModel2, redView, lightProj);
		//Light aggregation
		Tdraw(lpass, quad, lightFrame, camView,
			geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
			shadowFrame.depth, redColor, redView, lightProj, isPhong);

		//Green light
		ClearFramebuffer(shadowFrame);
		Tdraw(spass, spear, shadowFrame, spearModel, greenView, lightProj);
		Tdraw(spass, sphere, shadowFrame, sphereModel, greenView, lightProj);
		Tdraw(spass, quad, shadowFrame, wallModel, greenView, lightProj);
		//Tdraw(spass, plane, shadowFrame, planeModel, greenView, lightProj);
		//Tdraw(spass, plane2, shadowFrame, planeModel2, greenView, lightProj);
		//add the green light now.
		Tdraw(lpass, quad, lightFrame, camView,
			geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
			shadowFrame.depth, greenColor, greenView, lightProj, isPhong);

		//Blue light
		//ClearFramebuffer(shadowFrame);
		//Tdraw(spass, spear, shadowFrame, spearModel, blueView, lightProj);
		//Tdraw(spass, sphere, shadowFrame, sphereModel, blueView, lightProj);
		//Tdraw(spass, quad, shadowFrame, wallModel, blueView, lightProj);
		////Tdraw(spass, plane, shadowFrame, planeModel, blueView, lightProj);
		////add the blue light now.
		//Tdraw(lpass, quad, lightFrame, camView,
		//	geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
		//	shadowFrame.depth, blueColor, blueView, lightProj, isPhong);

		//Whites light
		//To do: add a light map
		//ClearFramebuffer(shadowFrame);
		//Tdraw(spass, spear, shadowFrame, spearModel, whiteView, lightProj);
		//Tdraw(spass, sphere, shadowFrame, sphereModel, whiteView, lightProj);
		//Tdraw(spass, quad, shadowFrame, wallModel, whiteView, lightProj);
		////Tdraw(spass, plane, shadowFrame, planeModel, whiteView, lightProj);
		////add the blue light now.
		//Tdraw(lpass, quad, lightFrame, camView,
		//	geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
		//	shadowFrame.depth, whiteColor, whiteView, lightProj, isPhong);


		//Post pass//
		ClearFramebuffer(postFrame); 
		Tdraw(postShader, quad, postFrame, lightFrame.colors[0], saturation, brightness, contrast, glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), 0.5f);

		//Test
		//Debug windows
		/*Texture debug_list[] = { geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3], geometryFrame.depth, lightFrame.colors[1], lightFrame.colors[2], shadowFrame.depth};
		for (int i = 0; i < sizeof(debug_list) / sizeof(Texture); ++i)
		{
			glm::mat4 mod = glm::translate(glm::vec3(-.75f + .5f*(i % 4), 0.75f - .5f*(i / 4), 0)) * glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			Tdraw(qdraw, quad, screen, debug_list[i], mod);
		}*/


		//Final Product
		//glm::mat4 mod = glm::translate(glm::vec3(-.5f, -0.5f, 0)) * glm::scale(glm::vec3(0.5f, 0.5f, 1.f));
		glm::mat4 mod = glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(1.0f, 1.0f, 1.f));
		Tdraw(qdraw, quad, screen, postFrame.colors[0], mod);
	}

	input.Term();
	time.Term();
	context.Term();
}