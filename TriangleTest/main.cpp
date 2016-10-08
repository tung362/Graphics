#include "crenderutils.h"
#include "RadialGradientGen.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "Procgen.h"
#include "GLM\ext.hpp"
#include <iostream>

struct RenderObject
{
	Geometry mesh;
	Texture norm, diff, spec;
	glm::mat4 model;
	static Framebuffer roFrame;
	// gpass shader for this object.
	// draw(...)
};

struct DirectionalLight
{
	glm::vec3 dir, color;
	float size;
	glm::mat4 getMatrix()
	{
		return glm::ortho<float>(-size, size, -size, size, -size, size)
		*glm::lookAt(-dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}
};

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

	//Generated objects
	//Texture noise = GenRadialGradient(64, glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), 0.5f);


	//Load objs//
	Texture spear_normal = LoadTex("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = LoadTex("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = LoadTex("../res/textures/soulspear_specular.tga");

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


	//Light matrix//
	glm::mat4 lightProj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 redView = glm::lookAt(glm::normalize(-glm::vec3(1, -1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 redColor = glm::vec4(1, 0, 0, 1);
	glm::mat4 greenView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 greenColor = glm::vec4(0, 1, 0, 1);


	while (context.Step())
	{
		time.Step();
		input.Step();
		spearModel = glm::rotate(time.GetTotalTime(), glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(1, 1, 1));

		//Geometry pass//
		ClearFramebuffer(geometryFrame);
		Tdraw(gpass, spear, geometryFrame, spearModel, camView, camProj, spear_diffuse, spear_normal, spear_specular);
		Tdraw(gpass, sphere, geometryFrame, sphereModel, camView, camProj, white, vertex_normals, white);
		Tdraw(gpass, quad, geometryFrame, wallModel, camView, camProj, white, vertex_normals, white);


		//Light pass//
		ClearFramebuffer(lightFrame);
		//Shadow prePass
		//RED LIGHT
		ClearFramebuffer(shadowFrame);
		Tdraw(spass, spear, shadowFrame, spearModel, redView, lightProj);
		Tdraw(spass, sphere, shadowFrame, sphereModel, redView, lightProj);
		Tdraw(spass, quad, shadowFrame, wallModel, redView, lightProj);
		//Light aggregation
		Tdraw(lpass, quad, lightFrame, camView,
			geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
			shadowFrame.depth, redColor, redView, lightProj);

		//Green light
		ClearFramebuffer(shadowFrame);
		Tdraw(spass, spear, shadowFrame, spearModel, greenView, lightProj);
		Tdraw(spass, sphere, shadowFrame, sphereModel, greenView, lightProj);
		Tdraw(spass, quad, shadowFrame, wallModel, greenView, lightProj);
		//add the green light now.
		Tdraw(lpass, quad, lightFrame, camView,
			geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3],
			shadowFrame.depth, greenColor, greenView, lightProj);


		//Post pass//
		ClearFramebuffer(postFrame); 
		Tdraw(postShader, quad, postFrame, lightFrame.colors[0], 1.0f, 1.0f, 1.0f, glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), 0.5f);

		//Test
		//Debug windows
		Texture debug_list[] = { geometryFrame.colors[0], geometryFrame.colors[1], geometryFrame.colors[2], geometryFrame.colors[3], geometryFrame.depth, lightFrame.colors[1], lightFrame.colors[2], shadowFrame.depth};
		for (int i = 0; i < sizeof(debug_list) / sizeof(Texture); ++i)
		{
			glm::mat4 mod = glm::translate(glm::vec3(-.75f + .5f*(i % 4), 0.75f - .5f*(i / 4), 0)) * glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			Tdraw(qdraw, quad, screen, debug_list[i], mod);
		}


		//Final Product
		glm::mat4 mod = glm::translate(glm::vec3(-.5f, -0.5f, 0)) * glm::scale(glm::vec3(0.5f, 0.5f, 1.f));
		Tdraw(qdraw, quad, screen, postFrame.colors[0], mod);
	}

	input.Term();
	time.Term();
	context.Term();
}