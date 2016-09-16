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
	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Color
	glEnableVertexAttribArray(2); //Normal
	glEnableVertexAttribArray(3); //TexCoord

	//index of the attribute, number of elements, type, normalized?
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::COLOR);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::NORMAL);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::TEXCOORD);

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

	int vsize = shapes[0].mesh.indices.size();
	Vertex   *verts = new Vertex[vsize];
	unsigned * tris = new unsigned[vsize];

	auto &ind = shapes[0].mesh.indices;
	for (int i = 0; i < vsize; ++i)
	{
		auto ind = shapes[0].mesh.indices[i];

		const float *n = &attrib.normals[ind.normal_index * 3];
		const float *p = &attrib.vertices[ind.vertex_index * 3];

		verts[i].position = glm::vec4(p[0], p[1], p[2], 1.f);
		verts[i].normal = glm::vec4(n[0], n[1], n[2], 0.f);

		if (ind.texcoord_index >= 0)
		{
			const float *t = &attrib.texcoords[ind.texcoord_index * 2];
			verts[i].texcoord = glm::vec2(t[0], t[1]);
		}

		tris[i] = i;
	}

	Geometry retval = MakeGeometry(verts, vsize, tris, vsize);

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

Texture MakeTex(unsigned width, unsigned height, unsigned format, const unsigned char * pixels)
{
	Texture retval = { 0, width, height, format };

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return retval;
}

Texture MakeTexF(unsigned square, const float * pixels)
{
	Texture retval = { 0, square, square, GL_RED }; //GL_RED, GL_RG, GL_RGB, GL_RGBA
	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, square, square, 0, GL_RED, GL_FLOAT, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return retval;
}

Texture LoadTex(const char * path)
{
	int w, h, f;
	unsigned char *p;

	Texture retval = {0,0,0,0};

	stbi_set_flip_vertically_on_load(true); // DirectX or OpenGL
	p = stbi_load(path, &w, &h, &f, STBI_default);

	if (!p) return retval;

	switch (f)
	{
	case STBI_grey : f = GL_RED;  break;
	case STBI_grey_alpha: f = GL_RG;   break;
	case STBI_rgb : f = GL_RGB;  break;
	case STBI_rgb_alpha	: f = GL_RGBA; break;
	}
	
	retval = MakeTex(w, h, f, p);
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

void draw(const Shader &s, const Geometry &g, const Texture &t, const float M[16], const float V[16], const float P[16], float time)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);


	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glUniform1f(3, time);

	// Minimum guaranteed is 8.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.handle);
	glUniform1i(4, 0);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

void drawPhong(const Shader &s, const Geometry &g, const float M[16], const float V[16], const float P[16])
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);


	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);



	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

void drawPhong(const Shader &s, const Geometry &g, const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	int i = 0;
	for (; i < t_count; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, T[i].handle);
		glUniform1i(3 + i, 0 + i);
	}

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

Framebuffer MakeFramebuffer(unsigned width, unsigned height, unsigned nColors)
{
	Framebuffer retval = { 0,width,height,nColors };

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	////////////////////////////////////////////////////////////////////////////////////
	retval.depth = MakeTex(width, height, GL_DEPTH_COMPONENT, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, retval.depth.handle, 0);
	////////////////////////////////////////////////////////////////////////////////////

	const GLenum attachments[8] =
	{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	for (int i = 0; i < nColors && i < 8; ++i)
	{
		retval.colors[i] = MakeTex(width, height, GL_RGBA, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, attachments[i], retval.colors[i].handle, 0);
	}
	glDrawBuffers(nColors, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}

void ClearFramebuffer(const Framebuffer &f)
{
	glBindFramebuffer(GL_FRAMEBUFFER, f.handle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FreeFramebuffer(Framebuffer &fb)
{
	for (unsigned i = 0; i < fb.nColors; ++i)
		FreeTexture(fb.colors[i]);

	glDeleteFramebuffers(1, &fb.handle);
	fb = { 0,0,0,0 };
}



void DrawFB(const Shader &s, const Geometry &g, const Framebuffer &f, const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, f.handle);
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glViewport(0, 0, f.width, f.height);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	for (int i = 0; i < t_count; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, T[i].handle);
		glUniform1i(3 + i, 0 + i);
	}


	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}