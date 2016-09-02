#define TINYOBJLOADER_IMPLEMENTATION
#include "Obj\tiny_obj_loader.h"
#include <random>
#include "gldecs.h"
#include "crenderutils.h"
#include "Vertex.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

Geometry MakeGeometry(const Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize)
{
	Geometry retval;
	retval.size = tsize;

	//Define the variables
	glGenBuffers(1, &retval.vbo); //store vertices
	glGenBuffers(1, &retval.ibo); //store indices(triangle)
	glGenVertexArrays(1, &retval.vao); //store attribute information

	//Scope the variables
	glBindVertexArray(retval.vao);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);

	//Initialize the variables
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

	//attribute let us tell openGL how the memory is laid out
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//index of the attribute, number of elements, type, normalized?
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) 16);

	//Unscope the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return retval;
}

void FreeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteBuffers(1, &geo.vao);
	geo = {0, 0, 0, 0};
}

Geometry LoadObj(const char *path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	Vertex   *verts = new Vertex[attrib.vertices.size() / 3];
	unsigned * tris = new unsigned[shapes[0].mesh.indices.size()];

	for (int i = 0; i < attrib.vertices.size() / 3; ++i)
	{
		verts[i] = { attrib.vertices[i * 3],
			attrib.vertices[i * 3 + 1],
			attrib.vertices[i * 3 + 2], 1 };

		verts[i].color[0] = rand() * 1.0f / RAND_MAX;
		verts[i].color[1] = rand() * 1.0f / RAND_MAX;
		verts[i].color[2] = rand() * 1.0f / RAND_MAX;
		verts[i].color[3] = 1;
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); ++i)
		tris[i] = shapes[0].mesh.indices[i].vertex_index;

	Geometry retval = MakeGeometry(verts, attrib.vertices.size() / 3,
		tris, shapes[0].mesh.indices.size());

	delete[] verts;
	delete[] tris;

	return retval;
}

Shader MakeShader(const char * vsource, const char * fsource)
{
	Shader retval;

	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);

	glCompileShader(vs);
	glCompileShader(fs);

	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);
	glLinkProgram(retval.handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void FreeShader(Shader &shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

std::string GetTextFromFile(const char *path)
{
	std::ifstream textFile{ path };
	std::string file_contents
	{
		std::istreambuf_iterator<char>(textFile),
		std::istreambuf_iterator<char>()
	};
	return file_contents;
}

Shader LoadShader(const char *vpath, const char *fpath)
{
	return MakeShader(GetTextFromFile(vpath).c_str(), GetTextFromFile(fpath).c_str());
}

void Draw(const Shader &shader, const Geometry &geometry)
{
	glUseProgram(shader.handle);

	//binding the vao also binds the IBO(tri) and VBO (verts)
	glBindVertexArray(geometry.vao);

	//Draw elements will draw the vertices that are currently bound
	//Using an array of indices
	//If an IBO is bound, we don't need to provide any indices
	glDrawElements(GL_TRIANGLES, geometry.size, GL_UNSIGNED_INT, 0);
}

void Draw(const Shader &shader, const Geometry &geometry, float time)
{
	glUseProgram(shader.handle);
	glBindVertexArray(geometry.vao);

	int loc = glGetUniformLocation(shader.handle, "time");
	glUniform1f(loc, time);

	glDrawElements(GL_TRIANGLES, geometry.size, GL_UNSIGNED_INT, 0);
}

void Draw(const Shader &s, const Geometry &g, const float m[16], const float v[16], const float p[16], float color2)
{
	glEnable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, p);
	glUniformMatrix4fv(1, 1, GL_FALSE, v);
	glUniformMatrix4fv(2, 1, GL_FALSE, m);

	int loc = glGetUniformLocation(s.handle, "color2");
	glUniform1f(loc, color2);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

Texture MakeTexture(unsigned width, unsigned height, unsigned format, const unsigned char * pixels)
{
	Texture retval = {0, width, height, format};
	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return retval;
}

Texture LoadText(const char * path)
{
	int w, h, f;
	unsigned char *p;

	Texture retval = { 0, 0, 0, 0 };

	stbi_set_flip_vertically_on_load(true);
	p = stbi_load(path, &w, &h, &f, STBI_default);

	if (!p) return retval;

	switch (f)
	{
	case STBI_grey: f = GL_RED; break;
	case STBI_grey_alpha: f = GL_RG; break;
	case STBI_rgb: f = GL_RGB; break;
	case STBI_rgb_alpha: GL_RGBA; break;
	}

	retval = MakeTexture(w, h, f, p);
	stbi_image_free(p);
	return retval;
}

void FreeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0, 0, 0, 0 };
}

void Draw(const Shader &s, const Geometry &g, const Texture &t, const float m[16], const float v[16], const float p[16], float color2)
{
	glEnable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, p);
	glUniformMatrix4fv(1, 1, GL_FALSE, v);
	glUniformMatrix4fv(2, 1, GL_FALSE, m);

	int loc = glGetUniformLocation(s.handle, "color2");
	glUniform1f(loc, color2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.handle);
	glUniform1i(4, 0);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}
