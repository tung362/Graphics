#include <iostream>

#include "Window.h"
#include "crenderutils.h"
#include "Gallery.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

void GenerateGrid(unsigned int rows, unsigned int cols, Gallery &gallery)
{
	size_t vsize = rows * cols;
	size_t tsize = (rows - 1) * (cols - 1) * 6;

	Vertex* verts = new Vertex[vsize];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			/*verts[r * cols + c].position[0] = (float)c;
			verts[r * cols + c].position[1] = 0;
			verts[r * cols + c].position[2] = (float)r;
			verts[r * cols + c].position[3] = 1;*/
			float vec4[4] = {(float)c, 0, (float)r, 1};
			memcpy_s(&verts[r * cols + c], sizeof(float) * 4, &vec4, sizeof(float)*4 );
		}
	}

	unsigned int* tris = new unsigned int[tsize];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			tris[index++] = r * cols + c;
			tris[index++] = (r + 1) * cols + c;
			tris[index++] = (r + 1) * cols + (c + 1);

			tris[index++] = r * cols + c;
			tris[index++] = (r + 1) * cols + (c + 1);
			tris[index++] = r * cols + (c + 1);
		}
	}

	//perlin noise
	int dims = 64;
	float* perlinNoise = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;
	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinNoise[y * dims + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinNoise[y * dims + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	//gallery.MakeTexture("Perlin", 64, 64,  );

	/*glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

	//gallery.LoadShader("SimplePerlin", "../res/shaders/PerlinVert.txt", "../res/shaders/PerlinFrag.txt");



	gallery.MakeObject("Grid", verts, vsize, tris, tsize);
}

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

	GenerateGrid(64, 64, gallery);

	Vertex verts[] = { {-1, -1, 0, 1}, {-1, 1, 0, 1},
					   {1, 1, 0, 1}, {1, -1, 0, 1}, };
	unsigned tris[] = {0, 1, 2, 2, 3, 0};
	gallery.MakeObject("quad", verts, 4, tris, 6);

	unsigned char pixels[] = {255, 255, 0};
	//Texture tex = MakeTexture(1, 1, 0x1907, pixels);
	gallery.LoadTexture("Forest" ,"../res/textures/forest.jpg");

	gallery.LoadObjectOBJ("Sphere", "../res/models/sphere.obj");
	gallery.LoadObjectOBJ("Cube", "../res/models/cube.obj");
	gallery.LoadObjectOBJ("Crystal", "../res/models/crystal.obj");
	gallery.LoadShader("Simple", "../res/shaders/SimpleVert.txt", "../res/shaders/SimpleFrag.txt");
	gallery.LoadShader("SimpleCamera", "../res/shaders/CameraVert.txt", "../res/shaders/CameraFrag.txt");
	gallery.LoadShader("SimpleTexture", "../res/shaders/TextureVert.txt", "../res/shaders/TextureFrag.txt");

	glm::mat4 proj, view, model, model2, model3;
	proj = glm::perspective(45.f, 1.f, 0.1f, 100.f);

	view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f),
					   glm::vec3(0.f, 0.f, 0.f),
					   glm::vec3(0.f, 1.f, 0.f));

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

		/*Draw(gallery.GetShader("SimpleTexture"), gallery.GetObject("Cube"), gallery.GetTexture("Forest"),
												glm::value_ptr(model), 
												glm::value_ptr(view), 
												glm::value_ptr(proj), colorswitch);

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Sphere"),
												glm::value_ptr(model2),
												glm::value_ptr(view),
												glm::value_ptr(proj), colorswitch);*/

		Draw(gallery.GetShader("SimpleCamera"), gallery.GetObject("Crystal"),
												glm::value_ptr(model3),
												glm::value_ptr(view),
												glm::value_ptr(proj), colorswitch);

		/*Draw(gallery.GetShader("SimpleTexture"), gallery.GetObject("quad"), gallery.GetTexture("Forest"),
			glm::value_ptr(model),
			glm::value_ptr(view),
			glm::value_ptr(proj), colorswitch);*/

		Draw(gallery.GetShader("SimpleTexture"), gallery.GetObject("Grid"), gallery.GetTexture("Forest"),
			glm::value_ptr(model),
			glm::value_ptr(view),
			glm::value_ptr(proj), colorswitch);

	}
	//FreeTexture(tex);

	gallery.Term();
	input.Term();
	time.Term();
	window.Term();
	return 0;
}