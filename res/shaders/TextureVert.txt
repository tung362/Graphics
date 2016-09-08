#version 430


layout(location = 0) in vec4 position;
layout(location = 3) in vec2 texcoord;

out vec2 vtexCoord;

layout(location = 0) uniform mat4 Projection;	
layout(location = 1) uniform mat4 View;		
layout(location = 2) uniform mat4 Model;	
layout(location = 3) uniform float time;
layout(location = 4) uniform sampler2D texMap;


void main()
{
	vtexCoord = texcoord;

	vec4 p = position;

	p.y = texture(texMap, texcoord).r;

	gl_Position = Projection * View * Model * p;
}