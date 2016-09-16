#pragma once
#include <iostream>
#include "Globjects.h"

Geometry MakeGeometry(const struct Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize);

void FreeGeometry(Geometry &);

Geometry LoadObj(const char *path);

Shader MakeShader(const char *vsource, const char *fsource);

std::string GetTextFromFile(const char *path);

void FreeShader(Shader &);

Shader LoadShader(const char *vpath, const char *fpath);

void Draw(const Shader &, const Geometry &);

void Draw(const Shader &, const Geometry &, float time);

void Draw(const Shader &, const Geometry &, const float m[16], const float v[16], const float p[16], float color2);

Texture MakeTex(unsigned width, unsigned height, unsigned format, const unsigned char *pixels);
Texture MakeTexF(unsigned square, const float *pixels);

Texture LoadTex(const char *path);

void FreeTexture(Texture &t);

void Draw(const Shader &, const Geometry &, const Texture &, const float m[16], const float v[16], const float p[16], float color2);
void draw(const Shader &, const Geometry &, const Texture &, const float M[16], const float V[16], const float P[16], float time);
void drawPhong(const Shader &, const Geometry &, const float M[16], const float V[16], const float P[16]);
void drawPhong(const Shader &, const Geometry &, const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count);

Framebuffer MakeFramebuffer(unsigned width, unsigned height, unsigned nColors);
void FreeFramebuffer(Framebuffer &);
void ClearFramebuffer(const Framebuffer &);

void DrawFB(const Shader &s, const Geometry &g, const Framebuffer &f, const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count);
