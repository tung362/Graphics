#version 430

in vec2 UV;
layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;
layout(location = 2)uniform mat4 model;

layout(location = 3)uniform sampler2D colorMap;		// combined diffuse + specular
layout(location = 4)uniform sampler2D depth;
out vec4 outColor;

uniform vec2 sDim = vec2(1280, 720);

void main()
{ 
	//Edge Detect
	vec4 yColor = 
	texture(colorMap,UV + vec2( 1, -1)/sDim)   +
	texture(colorMap,UV + vec2( 0, -1)/sDim)*2 +
	texture(colorMap,UV + vec2(-1, -1)/sDim)   -
	texture(colorMap,UV + vec2( 1,  1)/sDim)    -
	texture(colorMap,UV + vec2( 0,  1)/sDim)*2  -
	texture(colorMap,UV + vec2(-1,  1)/sDim);

	vec4 xColor = 
	texture(colorMap,UV + vec2(-1, -1)/sDim)   +
	texture(colorMap,UV + vec2(-1,  0)/sDim)*2 +
	texture(colorMap,UV + vec2(-1,  1)/sDim)   -
	texture(colorMap,UV + vec2( 1, -1)/sDim)   -
	texture(colorMap,UV + vec2( 1,  0)/sDim)*2 -
	texture(colorMap,UV + vec2( 1,  1)/sDim);

	outColor = sqrt(yColor * yColor + xColor * xColor);

	//Sharpen
	/*vec2 texelSize = 1.0f / sDim;

	vec4 specColorMod = texture(specularColor, UV) * (11 / 3);
	specColorMod += texture(specularColor, UV + vec2(0, texelSize.y)) * (-2 / 3);
	specColorMod += texture(specularColor, UV - vec2(0, texelSize.y)) * (-2 / 3);
	specColorMod += texture(specularColor, UV + vec2(texelSize.x, 0)) * (-2 / 3);
	specColorMod += texture(specularColor, UV - vec2(texelSize.x, 0)) * (-2 / 3);
	specColorMod.a = 1.0f;

	vec4 diffColorMod = texture(diffuseColor, UV) * (11 / 3);
	diffColorMod += texture(diffuseColor, UV + vec2(0, texelSize.y)) * (-2 / 3);
	diffColorMod += texture(diffuseColor, UV - vec2(0, texelSize.y)) * (-2 / 3);
	diffColorMod += texture(diffuseColor, UV + vec2(texelSize.x, 0)) * (-2 / 3);
	diffColorMod += texture(diffuseColor, UV - vec2(texelSize.x, 0)) * (-2 / 3);
	diffColorMod.a = 1.0f;

	outColor = specColorMod + diffColorMod;*/

	/*float focus = 1.5f;
	float focusNear = 1;
	float focusFar = 2;

	float depth = 1.5f;
	float result = 0;

	float f = 0;
	if (depth < focus)
		f = (depth - focus) / (focus - focusNear);
	else
		f = (depth - focus) / (focusFar - focus);

	result = min(1, abs(f));

	vec2 texelSize = 1.0f / sDim;

	vec4 specColorMod = texture(specularColor, UV);
	specColorMod += texture(specularColor, UV + vec2(0, texelSize.y)) * result;
	specColorMod += texture(specularColor, UV - vec2(0, texelSize.y)) * result;
	specColorMod += texture(specularColor, UV + vec2(texelSize.x, 0)) * result;
	specColorMod += texture(specularColor, UV - vec2(texelSize.x, 0)) * result;
	specColorMod.a = 1.0f;

	vec4 diffColorMod = texture(diffuseColor, UV);
	diffColorMod += texture(diffuseColor, UV + vec2(0, texelSize.y)) * result;
	diffColorMod += texture(diffuseColor, UV - vec2(0, texelSize.y)) * result;
	diffColorMod += texture(diffuseColor, UV + vec2(texelSize.x, 0)) * result;
	diffColorMod += texture(diffuseColor, UV - vec2(texelSize.x, 0)) * result;
	diffColorMod.a = 1.0f;

	outColor = specColorMod + diffColorMod;*/
}