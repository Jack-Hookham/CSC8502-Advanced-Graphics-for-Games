#version 150 core

//Basic fragment shader

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColour;
out vec2 texCoord;

void main(void)	{	
	fragColour = IN.colour;
}