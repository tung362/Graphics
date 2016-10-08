#include "RadialGradientGen.h"
#include "glmake.h"
#include "GLM\ext.hpp"

//vec4 RadialGradient(in sampler2D map, in vec2 UV)
//{
//	//Position where the circle starts
//	vec2 CenterPosition = vec2(0.5, 0.5);
//	vec4 retval = texture(map, UV);
//	float t = Distance(UV, CenterPosition) * 1.41421356237;
//	retval = lerp(gradientColorA, gradientColorB, t + (gradientSlider - 0.5) * 2);
//	return retval;
//}

glm::vec4 lerp(glm::vec4 start, glm::vec4 target, float percentage)
{
	return start * (1.0f - percentage) + target * percentage;
}

float Distance(glm::vec2 a, glm::vec2 b)
{
	glm::vec2 thedistance = a - b;
	float magnitude = sqrt(thedistance.x*thedistance.x + thedistance.y*thedistance.y);
	return magnitude;
}

Texture GenRadialGradient(unsigned size, glm::vec4 gradientColorA, glm::vec4 gradientColorB, float gradientSlider)
{
	float *pixels = new float[size*size];
	glm::vec2 CenterPosition = glm::vec2(0.5, 0.5);



	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < size; ++y)
		{
			float t = Distance(glm::vec2(x, y), CenterPosition) * 1.41421356237;
			float lerped = lerp(gradientColorA, gradientColorB, t + (gradientSlider - 0.5) * 2).x;
			pixels[y * size + x] = lerped;
		}
	}


	Texture retval = MakeTexF(size, pixels);
	delete[] pixels;
	return retval;
}
