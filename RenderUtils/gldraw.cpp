#include "gldecs.h"
#include "gldraw.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

void ClearFramebuffer(const Framebuffer & r)
{
	glBindFramebuffer(GL_FRAMEBUFFER, r.handle);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void UseShaderFlags(const Shader &s)
{
	if (s.depthTest)
		glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	if (s.faceCulling)
		glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);

	if (s.additiveBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}
	else glDisable(GL_BLEND);

}

void Tdraw_internal::Tdraw_begin(const Shader & s, const Geometry & g, const Framebuffer & r)
{
	glBindFramebuffer(GL_FRAMEBUFFER, r.handle);
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	UseShaderFlags(s);
	glViewport(0, 0, r.width, r.height);
}


void Tdraw_internal::Tdraw_close(const Shader & s, const Geometry & g, const Framebuffer & r)
{
	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, const glm::mat4 & val)
{
	glUniformMatrix4fv(idx, 1, GL_FALSE, glm::value_ptr(val));
	idx++;
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, const glm::vec3 & val)
{
	glUniform3fv(idx, 1, glm::value_ptr(val));
	idx++;
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, const glm::vec4 & val)
{
	glUniform4fv(idx, 1, glm::value_ptr(val));
	idx++;
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, int val)
{
	glUniform1i(idx, val);
	idx++;
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, float val)
{
	glUniform1f(idx, val);
	idx++;
}

void Tdraw_internal::Tdraw_format(size_t &idx, size_t &tex, const Texture & val)
{
	glActiveTexture(GL_TEXTURE0 + tex);
	glBindTexture(GL_TEXTURE_2D, val.handle);
	glUniform1i(idx, tex);
	idx++;
	tex++;
}

void Tdraw_internal::Tdraw_format(size_t & idx, size_t & tex, const Framebuffer & val)
{

	for (int i = 0; i < val.nColors; ++i)
		Tdraw_internal::Tdraw_format(idx, tex, val.colors[i]);


}