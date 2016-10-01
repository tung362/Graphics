#version 430

layout(location = 0) uniform sampler2D map;

in vec2 vUV;

out vec4 outColor;

vec4 sobel(in sampler2D map, in vec2 UV);
vec4 crossblur(in sampler2D map, in vec2 UV);
vec4 ColorCorrection(in sampler2D map, in vec2 UV);
vec3 lerp(vec3 start, vec3 target, float percentage);
//http://gamedev.stackexchange.com/questions/28782/hue-saturation-brightness-contrast-effect-in-hlsl
//http://www.opserver.de/ubb7/ubbthreads.php?ubb=showflat&Number=390152

void main()
{
	outColor = ColorCorrection(map, vUV);
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

vec4 ColorCorrection(in sampler2D map, in vec2 UV)
{
	vec4 retval = texture(map, vUV);
	float saturation = 0.5f;
	float Brightness = 0.5f;
	float Contrast = 0.5f;

    vec3 meanLuminosity = vec3(0.5f, 0.5f, 0.5f);
    vec3 rgb2greyCoeff = vec3(0.299f, 0.587f, 0.114f);
    vec3 brightened = vec3(retval.r, retval.g, retval.b) * brightness;
    float intensity = dot(brightened, rgb2greyCoeff);		
    vec3 saturated = lerp(vec3(intensity, intensity, intensity), brightened, saturation);
    vec3 contrasted = lerp(meanLuminosity, saturated, contrast);
    
    retval = vec4(contrasted.x, contrasted.y, contrasted.z, 1);
	return retval;
}

vec3 lerp(vec3 start, vec3 target, float percentage)
{
	return start * (1.0f - percentage) + target * percentage;
}