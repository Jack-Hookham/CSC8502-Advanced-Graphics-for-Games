#version 400 core

//tessellation control for destroying the cube

layout (triangles, fractional_even_spacing, ccw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex {
	vec4 colour;
	vec2 texCoord;
} IN[];

out Vertex {
	vec4 colour;
	vec2 texCoord;
} OUT;

void main () {
	vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

	vec3 combinedPos = p0 + p1 + p2;

	vec4 worldPos = modelMatrix * vec4 ( combinedPos , 1);

	gl_Position = projMatrix * viewMatrix * worldPos;
}