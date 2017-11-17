#version 150 core

in vec3 position;
in vec4 colour;
in vec2 texCoord;

out Vertex {
	vec4 colour;
	vec2 texCoord;
} OUT;

void main ( void ) {
	gl_Position = vec4 (position, 1.0);
	OUT.colour = vec4(1,1,1,1);
	OUT.texCoord = texCoord;
}