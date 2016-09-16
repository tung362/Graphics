#version 330


in  vec4  vPosition;
in  vec4  vColor;
out vec4  outColor;

uniform float time = 0.0;


void main ()
{ 
	//outColor = vColor;
	//outColor.r = (cos(time + vPosition.x * 10) + 1.0)/ 2.0;

	outColor = vPosition;

	vec4 cPos = vPosition;


	outColor.r = (cos(time*1.3 + cPos.x)+1)/2;
	outColor.g = (sin(time*1.3 + cPos.y)+1)/2;

	outColor.b = vPosition.x;

	float rad = sqrt(cPos.x*cPos.x + cPos.y*cPos.y);


	if(rad < (cos(time)+2)/2 &&  rad > (cos(time)+2)/4)
	{
	outColor  = vec4(1,1,1,1) - (sin(time*1.3 + cPos.y)+1)/2;
	}
}