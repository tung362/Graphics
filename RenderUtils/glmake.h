#pragma once

struct Geometry MakeGeometry(const struct Vertex *verts, size_t vsize, const unsigned int  *tris, size_t tsize);

void FreeGeometry(Geometry &);

struct Shader MakeShader(const char *vsource, const char *fsource, bool depth = true, bool add = false, bool face = true);

void FreeShader(Shader &);

struct Texture MakeTex(unsigned width, unsigned height, unsigned channels, const void *pixels, bool isFloat = false);

struct Texture MakeTexF(unsigned square, const float *pixels);
void FreeTexture(Texture &);

struct Framebuffer MakeFramebuffer(unsigned width, unsigned height, unsigned nColors, const bool *isfloat = nullptr, const int *channels = nullptr);
void FreeFramebuffer(Framebuffer &);