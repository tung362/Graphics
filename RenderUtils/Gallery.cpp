#include "Gallery.h"

bool Gallery::MakeShader(const char * name, const char * vsource, const char * fsource)
{
	shaders[name] = ::MakeShader(vsource, fsource);
	return true;
}

bool Gallery::LoadShader(const char * name, const char * vpath, const char * fpath)
{
	return false;
}

bool Gallery::MakeObject(const char * name, const Vertex * verts, size_t vsize, const unsigned * tris, size_t tsize)
{
	return false;
}

bool Gallery::LoadObjectOBJ(const char * name, const char * path)
{
	return false;
}

//const Geometry & Gallery::GetObject(const char * name)
//{
//	// TODO: insert return statement here
//}

const Shader & Gallery::GetShader(const char * name)
{
	return shaders[name];
}

bool Gallery::Init()
{
	return false;
}

bool Gallery::Term()
{
	for each(auto shader in shaders)
		FreeShader(shader.second);

	for each(auto object in objects)
		FreeGeometry(object.second);
	return false;
}
