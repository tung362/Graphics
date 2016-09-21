#pragma once
#include "GLM\glm.hpp"

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 texcoord;

	enum
	{
		POSITION = 0,
		COLOR = 16,
		NORMAL = 32,
		TEXCOORD = 48,
	};
};

const Vertex  quad_verts[4] = { { { -1,-1, 0, 1 }, { 1,1,1,0 }, { 0,0,1,0 }, { 0,0 } },
								{ { 1,-1, 0, 1 }, { 1,1,0,1 }, { 0,0,1,0 }, { 1,0 } },
								{ { 1, 1, 0, 1 }, { 1,0,1,1 }, { 0,0,1,0 }, { 1,1 } },
								{ { -1, 1, 0, 1 }, { 0,1,1,1 }, { 0,0,1,0 }, { 0,1 } } };

const unsigned quad_tris[6] = { 0, 1, 2,2, 3, 0 };