#include "crenderutils.h"

#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.Init(1280, 720);

	Geometry quad = MakeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = LoadObj("../res/models/soulspear.obj");

	Texture spear_normal = LoadTex("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = LoadTex("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = LoadTex("../res/textures/soulspear_specular.tga");

	Shader simple = LoadShader("../res/shaders/Simple.vert", "../res/shaders/Simple.frag");
	Shader gpass = LoadShader("../res/shaders/Gpass.vert", "../res/shaders/Gpass.frag");
	Shader lpass = LoadShader("../res/shaders/Lpass.vert", "../res/shaders/Lpass.frag");
	Shader post = LoadShader("../res/shaders/Quad.vert", "../res/shaders/Quad.frag");
	
	Framebuffer screen = { 0, 1280, 720 };
	Framebuffer gframe = MakeFramebuffer(1280, 720, 4);
	Framebuffer lframe = MakeFramebuffer(1280, 720, 2);

	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	float time = 0;
	while (context.Step())
	{
		time += 0.016f;
		ClearFramebuffer(gframe);
		ClearFramebuffer(lframe);

		model = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));

		Tdraw(gpass, spear, gframe, model, view, proj, spear_diffuse, spear_normal, spear_specular);

		// Lighting pass
		Tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], gframe.colors[1],
			gframe.colors[2], gframe.colors[3],
			gframe.depth);

		// Debug Rendering Stuff.
		for (int i = 0; i < 4; ++i)
		{
			glm::mat4 mod =
				glm::translate(glm::vec3(-.75f + .5*i, 0.75f, 0)) *
				glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			Tdraw(post, quad, screen, gframe.colors[i], mod);
		}

		glm::mat4 mod =
			glm::translate(glm::vec3(-.75f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		Tdraw(post, quad, screen, gframe.depth, mod);

		mod =
			glm::translate(glm::vec3(-.25f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		Tdraw(post, quad, screen, lframe.colors[0], mod);

		mod =
			glm::translate(glm::vec3(.25f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		Tdraw(post, quad, screen, lframe.colors[1], mod);
	}

	context.Term();
}