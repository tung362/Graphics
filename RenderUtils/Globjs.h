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
	
	bool depthTest;
	bool additiveBlend;
	bool faceCulling;
};

struct Texture
{
	unsigned handle;
	int width, height, format;
	//1 channel, gray scale
	//2 channel,
	//3 channel,
	//4 channel, with alpha
};

struct Framebuffer
{
	unsigned handle;
	unsigned width, height, nColors;
	Texture depth;
	Texture colors[8];
};