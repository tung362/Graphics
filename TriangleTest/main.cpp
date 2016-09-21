#include "crenderutils.h"

#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.Init(1280, 720);

	Framebuffer screen = { 0,1280,720 };

	Geometry quad = MakeGeometry(quad_verts, 4, quad_tris, 6);

	Shader simple = LoadShader("../res/shaders/simple.vert", "../res/shaders/simple.frag");

	Geometry spear = LoadObj("../res/models/soulspear.obj");

	Texture spear_normal = LoadTex("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = LoadTex("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = LoadTex("../res/textures/soulspear_specular.tga");

	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	while (context.Step())
	{
		Tdraw(simple, spear, screen, model, view, proj, spear_diffuse, spear_normal, spear_specular);
	}

	context.Term();
}