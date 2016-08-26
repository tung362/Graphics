#include "gldecs.h"
#include "crenderutils.h"
#include "Vertex.h"

Geometry MakeGeometry(const Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize)
{
	Geometry retval;
	retval.size = tsize;

	//Define the variables
	glGenBuffers(1, &retval.vbo); //store vertices
	glGenBuffers(1, &retval.ibo); //store indices(triangle)
	glGenVertexArrays(1, &retval.vao); //store attribute information

	//Scope the variables
	glBindVertexArray(retval.vao);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);

	//Initialize the variables
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

	//attribute let us tell openGL how the memory is laid out
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//index of the attribute, number of elements, type, normalized?
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) 16);

	//Unscope the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return retval;
}

void FreeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteBuffers(1, &geo.vao);
	geo = {0, 0, 0, 0};
}

Shader MakeShader(const char * vsource, const char * fsource)
{
	Shader retval;

	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);

	glCompileShader(vs);
	glCompileShader(fs);

	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);
	glLinkProgram(retval.handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void FreeShader(Shader &shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

void Draw(const Shader &shader, const Geometry &geometry)
{
	glUseProgram(shader.handle);

	//binding the vao also binds the IBO(tri) and VBO (verts)
	glBindVertexArray(geometry.vao);

	//Draw elements will draw the vertices that are currently bound
	//Using an array of indices
	//If an IBO is bound, we don't need to provide any indices
	glDrawElements(GL_TRIANGLES, geometry.size, GL_UNSIGNED_INT, 0);
}
