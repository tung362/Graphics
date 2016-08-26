#include "Window.h"
#include "crenderutils.h"
#include "Vertex.h"

int main()
{
	Window window;
	window.Init();

	Vertex vert[3] = { 
						{{0, .5f, 0, 1}, {1.0, 0.0, 0.0, 0.4}},
						{{.5f, -.5f, 0, 1}, { 0.0, 1.0, 0.0, 0.4 }},
						{{-.5f, -.5f, 0, 1},{ 0.0, 0.0, 1.0, 0.4 }}
					};
	Vertex vert2[3] = {
						{{ 0 - 0.3, .5f - 0.3, 0, 1 },{ 1.0, 0.0, 0.0, 0.4 }},
						{{ .5f - 0.3, -.5f - 0.3, 0, 1 },{ 0.0, 1.0, 0.0, 0.4 }},
						{{ -.5f - 0.3, -.5f - 0.3, 0, 1 },{ 0.0, 0.0, 1.0, 0.4 }}
					};
	unsigned tris[3] = {0, 1, 2};

	const char *vsource = 
		"#version 330\n"
		"layout(location = 0)in vec4 position;"
		"layout(location = 1)in vec4 color;"
		"out vec4 vColor;"
		"void main() { vColor = color; gl_Position = position; } ";
	const char *fsource =
		"#version 150\n"
		"in vec4 vColor;"
		"out vec4 outColor;"
		"void main () { outColor = vColor; } ";

	Geometry geometry = MakeGeometry(vert, 3, tris, 3);
	Shader shader = MakeShader(vsource, fsource);

	Geometry geometry2 = MakeGeometry(vert2, 3, tris, 3);

	while (window.Step())
	{
		Draw(shader, geometry);
		Draw(shader, geometry2);
	}

	FreeGeometry(geometry);
	FreeGeometry(geometry2);
	FreeShader(shader);
	window.Term();
	return 0;
}