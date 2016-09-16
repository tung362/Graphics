#version 330

// locations are the 'attributes' from the vertex
// in brings data in from the previous

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

// out sends data to the next step of the pipeline
out vec4 vColor;
out vec4 vPosition;

uniform float time = 0.0;

void main()
{
	vColor = color;
	vPosition = position;
	gl_Position = position;


	//gl_Position.y += cos(time + position.x)/2;

	//gl_Position.w = sin(cos((time + position.y)*2)*2+1)/10;
}