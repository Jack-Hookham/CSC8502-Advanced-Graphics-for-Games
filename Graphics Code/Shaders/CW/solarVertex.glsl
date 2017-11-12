#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform vec4 nodeColour;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex
{
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3	binormal;
	vec3 	worldPos;
} OUT;

void main(void)
{	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.colour = nodeColour;

	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
}