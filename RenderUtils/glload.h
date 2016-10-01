#pragma once

struct Shader LoadShader(const char *vpath, const char *fpath, bool depth = true, bool add = false, bool face = true);

struct Texture LoadTex(const char *path);

struct Geometry LoadObj(const char *path);