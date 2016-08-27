#pragma once
struct Geometry
{
	unsigned vbo, ibo, vao, size;
	//Vertex Buffer Object : array of vertices
	//Index Buffer Object : array of triangles
	//Vertex Array Object : groups the two with some formatting
	//Size : number of triangles
};

struct Shader
{
	unsigned handle;
};