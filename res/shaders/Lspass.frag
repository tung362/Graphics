#version 430

// Vertex Data
in vec2 vUV;
in vec4 vNormal;
in vec4 VPosition;

// Camera Data
layout(location = 0) uniform mat4 view;

// Gpass Data
layout(location = 1) uniform sampler2D albedoMap;
layout(location = 2) uniform sampler2D normalMap;
layout(location = 3) uniform sampler2D specularMap;
layout(location = 4) uniform sampler2D positionMap;

// Shadow Pass Data
layout(location = 5) uniform sampler2D shadowMap;
uniform float shadowBias = 0.1f;

// Light Data
layout(location = 6) uniform vec4 lCol;
layout(location = 7) uniform mat4 lightView;
layout(location = 8) uniform mat4 lightProj;

// Framebuffer Outputs
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outSpecular;

layout(location = 9) uniform bool toggle;


// Simple matrix that converts from clip space (-1,1) to UV space (0,1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

void main()
{		 
	vec3 L = normalize((view * lightView[2]).xyz);
	vec3 N = normalize(texture(normalMap, vUV).xyz);
	vec4 P = texture(positionMap,vUV);
	//Rim shading
	vec3 V = normalize(-P.rgb);
	float EYE = 1.0 - max(dot(V, N), 0.0);

	//Shadow Map calculations//
	//VIEW -> WORLD -> LIGHT -> CLIP -> UV
	vec4 sUV = clipToUV * lightProj * lightView * inverse(view) * vec4(P.xyz,1);

	//compare the sampled Z value against our projected Z position.
	//if the sample is closer, we don't draw in the shadow.
	if(texture(shadowMap, sUV.xy).r < sUV.z - shadowBias)
		discard;

	if(toggle)
	{
		//Phong calculations//
		vec3 R = reflect(L, N);
		vec3 E = normalize(view[3].xyz + P.xyz);
		float sP = 2;

		float lamb = max(0,-dot(L, N));
		float spec = max(0,-dot(E, R));
		if(spec > 0)
				spec = pow(spec, sP);

		vec3 rim = vec3(smoothstep(0.8, 1.0, EYE));
		//vec4(EYE, EYE,EYE, 1);

		//Final Result
		outAlbedo = texture(albedoMap,   vUV) * lamb * lCol;
		outSpecular = texture(specularMap, vUV) * spec * lCol;
		outColor =  outAlbedo + outSpecular + vec4(rim.x, rim.y, rim.z, 1);

		if((outAlbedo + outSpecular).rgb == vec3(0, 0, 0)) outColor = outAlbedo + outSpecular;


		//Metal
		//float intensity = dot(L,normalize(N));
		//if (intensity > 0.95)
		//	outColor = vec4(1, 1, 1, 1);
		//else if (intensity > 0.5)
		//	outColor = vec4(0.2, 0.2, 0.2, 1);
		//else if (intensity > 0.25)
		//	outColor = vec4(0, 0, 0, 1);
		//else
		//	outColor = vec4(0, 0, 0, 1);
	}
	else
	{
		vec3 lightDir = normalize((view * lightView[2]).xyz);
		vec3 normal = normalize(texture(normalMap, vUV).xyz);

		vec3 color = texture(albedoMap, vUV).rgb;

		//Ambient
		vec3 ambient = 0.05 * color;

		//Diffuse
		vec3 diffuse = max(dot(lightDir, normal), 0.0) * color;

		// Specular
		vec3 viewDir = normalize(VPosition.xyz);
		vec3 reflectDir = reflect(lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		vec3 specular = vec3(0.4, 0.4, 0.4) * spec; //Specular color

		outAlbedo   = texture(albedoMap,   vUV) * vec4(diffuse.xyz, 1);
		outSpecular = texture(specularMap, vUV) * vec4(specular.xyz, 1);
		outColor = outAlbedo + outSpecular;//ambient
	}
}