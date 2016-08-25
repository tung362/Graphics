#pragma once
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

struct Shader
{
	unsigned handle;
};

Shader MakeShader(const char *vsource, const char *fsource);

void FreeShader(Shader &);

void Draw(const Shader &, const Geometry &);