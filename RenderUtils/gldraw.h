#pragma once
#include "globjs.h"
#include "GLM\fwd.hpp"
#include <cstring>

void ClearFramebuffer(const Framebuffer &r);

namespace Tdraw_internal
{
	void Tdraw_begin(const Shader &s, const Geometry &g, const Framebuffer &r);
	void Tdraw_close(const Shader &s, const Geometry &g, const Framebuffer &r);

	void Tdraw_format(size_t &idx, size_t &tex, const glm::mat4 &val);
	void Tdraw_format(size_t &idx, size_t &tex, const glm::vec4 &val);
	void Tdraw_format(size_t &idx, size_t &tex, const glm::vec3 &val);
	void Tdraw_format(size_t &idx, size_t &tex, int   val);
	void Tdraw_format(size_t &idx, size_t &tex, float val);
	void Tdraw_format(size_t &idx, size_t &tex, const Texture &val);
	void Tdraw_format(size_t &idx, size_t &tex, const Framebuffer &val);


	//= std::enable_if_t<!std::is_pointer<T>::value, T>
	template<typename T, typename ...U>
	void Tdraw_unpack(size_t idx, size_t tex, const T &val, U &&...uniforms)
	{
		Tdraw_format(idx, tex, val);
		Tdraw_unpack(idx, tex, uniforms...);
	}

	template<typename T>
	void Tdraw_unpack(size_t idx, size_t tex, const T &val)
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