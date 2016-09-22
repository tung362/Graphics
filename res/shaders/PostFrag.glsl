#version 430

out vec4 outColor;

in vec2 vUV;

layout(location = 0) uniform sampler2D diffuseMap;
layout(location = 1) uniform sampler2D specularMap;

vec4 sobel(in sampler2D map, in vec2 UV);
vec4 crossblur(in sampler2D map, in vec2 UV);

void main()
{
	outColor = crossblur(diffuseMap, vUV) + sobel(specularMap, vUV);
}

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





//#version 430

//in vec2 UV;
//layout(location = 0)uniform mat4 proj;
//layout(location = 1)uniform mat4 view;
//layout(location = 2)uniform mat4 model;

//layout(location = 3)uniform sampler2D colorMap;		// combined diffuse + specular
//layout(location = 4)uniform sampler2D depth;
//out vec4 outColor;

//uniform vec2 sDim = vec2(1280, 720);

//void main()
//{ 
//	//Edge Detect
//	vec4 yColor = 
//	texture(colorMap,UV + vec2( 1, -1)/sDim)   +
//	texture(colorMap,UV + vec2( 0, -1)/sDim)*2 +
//	texture(colorMap,UV + vec2(-1, -1)/sDim)   -
//	texture(colorMap,UV + vec2( 1,  1)/sDim)    -
//	texture(colorMap,UV + vec2( 0,  1)/sDim)*2  -
//	texture(colorMap,UV + vec2(-1,  1)/sDim);

//	vec4 xColor = 
//	texture(colorMap,UV + vec2(-1, -1)/sDim)   +
//	texture(colorMap,UV + vec2(-1,  0)/sDim)*2 +
//	texture(colorMap,UV + vec2(-1,  1)/sDim)   -
//	texture(colorMap,UV + vec2( 1, -1)/sDim)   -
//	texture(colorMap,UV + vec2( 1,  0)/sDim)*2 -
//	texture(colorMap,UV + vec2( 1,  1)/sDim);

//	outColor = sqrt(yColor * yColor + xColor * xColor);

//	//Sharpen
//	/*vec2 texelSize = 1.0f / sDim;

//	vec4 specColorMod = texture(specularColor, UV) * (11 / 3);
//	specColorMod += texture(specularColor, UV + vec2(0, texelSize.y)) * (-2 / 3);
//	specColorMod += texture(specularColor, UV - vec2(0, texelSize.y)) * (-2 / 3);
//	specColorMod += texture(specularColor, UV + vec2(texelSize.x, 0)) * (-2 / 3);
//	specColorMod += texture(specularColor, UV - vec2(texelSize.x, 0)) * (-2 / 3);
//	specColorMod.a = 1.0f;

//	vec4 diffColorMod = texture(diffuseColor, UV) * (11 / 3);
//	diffColorMod += texture(diffuseColor, UV + vec2(0, texelSize.y)) * (-2 / 3);
//	diffColorMod += texture(diffuseColor, UV - vec2(0, texelSize.y)) * (-2 / 3);
//	diffColorMod += texture(diffuseColor, UV + vec2(texelSize.x, 0)) * (-2 / 3);
//	diffColorMod += texture(diffuseColor, UV - vec2(texelSize.x, 0)) * (-2 / 3);
//	diffColorMod.a = 1.0f;

//	outColor = specColorMod + diffColorMod;*/

//	/*float focus = 1.5f;
//	float focusNear = 1;
//	float focusFar = 2;

//	float depth = 1.5f;
//	float result = 0;

//	float f = 0;
//	if (depth < focus)
//		f = (depth - focus) / (focus - focusNear);
//	else
//		f = (depth - focus) / (focusFar - focus);

//	result = min(1, abs(f));

//	vec2 texelSize = 1.0f / sDim;

//	vec4 specColorMod = texture(specularColor, UV);
//	specColorMod += texture(specularColor, UV + vec2(0, texelSize.y)) * result;
//	specColorMod += texture(specularColor, UV - vec2(0, texelSize.y)) * result;
//	specColorMod += texture(specularColor, UV + vec2(texelSize.x, 0)) * result;
//	specColorMod += texture(specularColor, UV - vec2(texelSize.x, 0)) * result;
//	specColorMod.a = 1.0f;

//	vec4 diffColorMod = texture(diffuseColor, UV);
//	diffColorMod += texture(diffuseColor, UV + vec2(0, texelSize.y)) * result;
//	diffColorMod += texture(diffuseColor, UV - vec2(0, texelSize.y)) * result;
//	diffColorMod += texture(diffuseColor, UV + vec2(texelSize.x, 0)) * result;
//	diffColorMod += texture(diffuseColor, UV - vec2(texelSize.x, 0)) * result;
//	diffColorMod.a = 1.0f;

//	outColor = specColorMod + diffColorMod;*/
//}