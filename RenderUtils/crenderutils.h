#pragma once
#include <iostream>
struct Vertex;

struct Geometry
{
	unsigned vbo, ibo, vao, size;
	//Vertex Buffer Object : array of vertices
	//Index Buffer Object : array of triangles
	//Vertex Array Object : groups the two with some formatting
	//Size : number of triangles
};

Geometry MakeGeometry(const Vertex *verts, size_t vsize,
					  const unsigned int *tris, size_t tsize);

void FreeGeometry(Geometry &);

Geometry LoadObj(const char *path);

struct Shader
{
	unsigned handle;
};
Shader MakeShader(const char *vsource, const char *fsource);
std::string GetTextFromFile(const char *path);
void FreeShader(Shader &);
Shader LoadShader(const char *vpath, const char *fpath);

void Draw(const Shader &, const Geometry &);