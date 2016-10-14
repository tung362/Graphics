#version 430

layout(location = 0) uniform sampler2D map;
layout(location = 1) uniform float colorCorrectionSaturation;
layout(location = 2) uniform float colorCorrectionBrightness;
layout(location = 3) uniform float colorCorrectionContrast;
layout(location = 4) uniform vec4 gradientColorA;
layout(location = 5) uniform vec4 gradientColorB;
layout(location = 6) uniform float gradientSlider;

in vec2 vUV;

out vec4 outColor;

/*ADD CUSTOM MATH FUNCTIONS HERE >:V*/
vec3 lerp(vec3 start, vec3 target, float percentage)
{
	return start * (1.0f - percentage) + target * percentage;
}

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
/*END OF CUSTOM MATH FUNCTIONS D:*/


/*ADD CUSTOM SHADER FUNCTIONS HERE >:V*/
vec4 crossblur(in sampler2D map, in vec2 UV)
{
	vec2 sDim = textureSize(map,0).xy;
	vec4 retval = vec4(0,0,0,0);
	for(int i = -9; i <= 9; ++i)
	{
		retval += texture(map, UV + vec2( i, 0)/sDim);
		retval += texture(map, UV + vec2( 0, i)/sDim);
	}
	return retval / 36;
}

vec4 sobel(in sampler2D map, in vec2 UV)
{
	vec2 sDim = textureSize(map,0).xy;
	vec4 yColor = 
	texture(map,UV + vec2( 1, -1)/sDim)   +
	texture(map,UV + vec2( 0, -1)/sDim)*2 +
	texture(map,UV + vec2(-1, -1)/sDim)   -
	texture(map,UV + vec2( 1,  1)/sDim)    -
	texture(map,UV + vec2( 0,  1)/sDim)*2  -
	texture(map,UV + vec2(-1,  1)/sDim);

	vec4 xColor = 
	texture(map,UV + vec2(-1, -1)/sDim)   +
	texture(map,UV + vec2(-1,  0)/sDim)*2 +
	texture(map,UV + vec2(-1,  1)/sDim)   -
	texture(map,UV + vec2( 1, -1)/sDim)   -
	texture(map,UV + vec2( 1,  0)/sDim)*2 -
	texture(map,UV + vec2( 1,  1)/sDim);

	return sqrt(yColor * yColor + xColor * xColor);
}

vec4 ColorCorrection(in sampler2D map, in vec2 UV)
{
	vec4 retval = texture(map, vUV);
    vec3 meanLuminosity = vec3(0.5f, 0.5f, 0.5f);
    vec3 rgb2greyCoeff = vec3(0.299f, 0.587f, 0.114f);
    vec3 brightened = vec3(retval.x, retval.y, retval.z) * colorCorrectionBrightness;
    float intensity = dot(brightened, rgb2greyCoeff);		
    vec3 saturated = lerp(vec3(intensity, intensity, intensity), brightened, colorCorrectionSaturation);
    vec3 contrasted = lerp(meanLuminosity, saturated, colorCorrectionContrast);
	retval = vec4(contrasted.x, contrasted.y, contrasted.z, retval.w);
	return retval;
}
vec4 ColorCorrection(vec4 color)
{
	vec4 retval = color;
    vec3 meanLuminosity = vec3(0.5f, 0.5f, 0.5f);
    vec3 rgb2greyCoeff = vec3(0.299f, 0.587f, 0.114f);
    vec3 brightened = vec3(retval.x, retval.y, retval.z) * colorCorrectionBrightness;
    float intensity = dot(brightened, rgb2greyCoeff);		
    vec3 saturated = lerp(vec3(intensity, intensity, intensity), brightened, colorCorrectionSaturation);
    vec3 contrasted = lerp(meanLuminosity, saturated, colorCorrectionContrast);
	retval = vec4(contrasted.x, contrasted.y, contrasted.z, retval.w);
	return retval;
}

vec4 Glow(in sampler2D map, in vec2 UV)
{
	vec4 retval = texture(map, UV);
	vec4 sum = vec4(0, 0, 0, 0);

	for(int x = -11; x <= 11; x++)
	{
		for(int y = -11; y <= 11; y++)
		{
			//Combine Colors
			vec2 offset = vec2(x, y) * 0.001; //Smoothness of offsets
			sum += texture2D(map, UV + offset);
		}
	}

	return (sum / 450) + retval; //Blur
}

vec4 RadialGradient(in sampler2D map, in vec2 UV)
{
	//Position where the circle starts
	vec2 CenterPosition = vec2(0.5, 0.5);
	vec4 retval = texture(map, UV);
	float t = Distance(UV, CenterPosition) * 1.41421356237;
	retval = lerp(gradientColorA, gradientColorB, t + (gradientSlider - 0.5) * 2);
	return retval;
}
/*END OF CUSTOM SHADER FUNCTIONS D:*/


void main()
{
	outColor = Glow(map, vUV);
	outColor += ColorCorrection(map, vUV);
	//outColor += RadialGradient(map, vUV);
	//outColor += sobel(map, vUV);
	//outColor += crossblur(map, vUV);
}