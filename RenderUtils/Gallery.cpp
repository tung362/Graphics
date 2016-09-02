#include "gldecs.h"
#include "crenderutils.h"
#include "Gallery.h"

bool Gallery::MakeShader(const char * name, const char * vsource, const char * fsource)
{
	if (!shaders.count(std::string(name)))
	{
		shaders[name] = ::MakeShader(vsource, fsource);
		return true;
	}
	else
	{
		fprintf(stderr, "Shader %s already exists!", name);
		return false;
	}
}

bool Gallery::LoadShader(const char * name, const char * vpath, const char * fpath)
{
	if (!shaders.count(name))
	{
		shaders[name] = ::LoadShader(vpath, fpath);

		if (shaders[name].handle == 0)
		{
			fprintf(stderr, "Shader %s failed to load correctly!\n", name);
			shaders.erase(name);
		}

		fprintf(stderr, "Shader %s successfully loaded!\n", name);
		return true;
	}
	else
	{
		fprintf(stderr, "Shader %s already exists!", name);
		return false;
	}
}

bool Gallery::MakeObject(const char * name, const Vertex * verts, size_t vsize, const unsigned * tris, size_t tsize)
{
	objects[name] = MakeGeometry(verts, vsize, tris, tsize);
	return true;
}

bool Gallery::LoadObjectOBJ(const char * name, const char * path)
{
	objects[name] = LoadObj(path);
	return true;
}

bool Gallery::LoadTexture(const char * name, const char * path)
{
	textures[name] = LoadText(path);
	return true;
}

const Shader &Gallery::GetShader(const char * name)
{
	return shaders[name];
}

const Texture & Gallery::GetTexture(const char * name)
{
	return textures[name];
}

const Geometry &Gallery::GetObject(const char * name)
{
	return objects[name];
}


bool Gallery::Term()
{
	// loop through my maps and call my free functions
	for each(auto shader in shaders)
		FreeShader(shader.second);

	for each(auto object in objects)
		FreeGeometry(object.second);

	return true;
}

bool Gallery::Init()
{
	return true;
}