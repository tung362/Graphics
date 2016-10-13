#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 texcoord;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;

layout(location = 6) uniform int shapeType; //0 = regular, 1 = code generated height map, 2 = height map from file
layout(location = 7) uniform float mountainScale;
layout(location = 8) uniform float mountainX;
layout(location = 9) uniform float mountainY;
layout(location = 10) uniform sampler2D heightMap;

out vec2 vUV;
out vec3 vNormal;
out vec3 vPosition;

vec4 lerp(vec4 start, vec4 target, float percentage)
{
	return start * (1.0f - percentage) + target * percentage;
}

float Distance(vec2 a, vec2 b)
{
	vec2 thedistance = a - b;
	float magnitude = sqrt(thedistance.x*thedistance.x + thedistance.y*thedistance.y);
	return magnitude;
}

void main()
{
	vNormal   = (view * model * normal).xyz;
	vPosition = (view * model * position).xyz;
	vUV = texcoord;
	if(shapeType == 1)
	{
		//Generates a radial gradient heightmap from white to black, uses red to determine the height
		vec2 CenterPosition = vec2(mountainX, mountainY);
		float t = Distance(texcoord, CenterPosition) * 1.41421356237;
		vec4 p = position;
		p.g = lerp(vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), t + (0.5 - 0.5) * 2).r * mountainScale;

		gl_Position = proj * view * model * p;
	}
	else if(shapeType == 2)
	{
		vec4 p = position;
		p.g = texture(heightMap, texcoord).r * mountainScale;
		gl_Position = proj * view * model * p;
	}
	else gl_Position = proj * view * model * position;
}