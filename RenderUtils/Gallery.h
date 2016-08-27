#pragma once
#include <string>
#include <map>

#include "crenderutils.h"
#include "Vertex.h"

class Gallery
{
	std::map<std::string, Geometry> objects;
	std::map<std::string, Shader> shaders;

public:
	bool MakeShader(const char* name, const char* vsource, const char *fsource);
	bool LoadShader(const char* name, const char* vpath, const char *fpath);

	bool MakeObject(const char *name, const Vertex *verts, size_t vsize,
									const unsigned *tris, size_t tsize);

	bool LoadObjectOBJ(const char *name, const char *path);

	const Geometry &GetObject(const char *name);
	const Shader &GetShader(const char *name);

	bool Init();
	bool Term();
};