#pragma once

struct Geometry MakeGeometry(const struct Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize);

void FreeGeometry(Geometry &);

struct Shader MakeShader(const char *vsource, const char *fsource);
void FreeShader(Shader &);

struct Texture MakeTex(unsigned width, unsigned height, unsigned format, const unsigned char *pixels);
struct Texture MakeTexF(unsigned square, const float *pixels);
void FreeTexture(Texture &);

struct Framebuffer MakeFramebuffer(unsigned width, unsigned height, unsigned nColors);
void FreeFramebuffer(Framebuffer &);