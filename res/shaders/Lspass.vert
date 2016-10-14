#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 texcoord;

out vec2 vUV;
out vec4 vNormal;
out vec4 VPosition;


void main()
{
	vUV = texcoord;
	vNormal = normal;
	VPosition = position;
	gl_Position = position;
}