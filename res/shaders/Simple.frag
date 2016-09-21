#version 430

out vec4 outColor;

in vec4 vPosition;
in vec2 vUV;

layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 4) uniform sampler2D normalMap;
layout(location = 5) uniform sampler2D specularMap;

void main()
{
	outColor = texture(diffuseMap, vUV) + texture(normalMap, vUV);
}