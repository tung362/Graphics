#pragma once
#include "globjs.h"
#include "GLM\fwd.hpp"
#include <cstring>

void ClearFramebuffer(const Framebuffer &r);

namespace Tdraw_internal
{
	void Tdraw_begin(const Shader &s, const Geometry &g, const Framebuffer &r);
	void Tdraw_close(const Shader &s, const Geometry &g, const Framebuffer &r);

	size_t Tdraw_format(size_t idx, size_t tex, const glm::mat4 &val);
	size_t Tdraw_format(size_t idx, size_t tex, const glm::vec3 &val);
	size_t Tdraw_format(size_t idx, size_t tex, int   val);
	size_t Tdraw_format(size_t idx, size_t tex, float val);
	size_t Tdraw_format(size_t idx, size_t tex, const Texture &val);


	template<typename T, typename ...U>
	void Tdraw_unpack(size_t idx, size_t tex, T val, U &&...uniforms)
	{
		tex += Tdraw_format(idx, tex, val);
		Tdraw_unpack(idx + 1, tex, uniforms...);
	}

	template<typename T>
	void Tdraw_unpack(size_t idx, size_t tex, T val)
	{
		Tdraw_format(idx, tex, val);
	}
}

template<typename ...U>
void Tdraw(const Shader &s, const Geometry &g, const Framebuffer &r, U ... uniforms)
{
	Tdraw_internal::Tdraw_begin(s, g, r);

	Tdraw_internal::Tdraw_unpack(0, 0, uniforms...);

	Tdraw_internal::Tdraw_close(s, g, r);
}

inline void Tdraw(const Shader &s, const Geometry &g, const Framebuffer &r)
{
	Tdraw_internal::Tdraw_begin(s, g, r);
	Tdraw_internal::Tdraw_close(s, g, r);
}