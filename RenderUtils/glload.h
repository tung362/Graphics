#pragma once

struct Shader LoadShader(const char *vpath, const char *fpath);

struct Texture LoadTex(const char *path);

struct Geometry LoadObj(const char *path);