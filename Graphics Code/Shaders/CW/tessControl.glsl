#version 400 core

//split the cube using tessellation and inner/outer values

layout (vertices = 4) out;

in Vertex {
	vec4 colour;
	vec2 texCoord;
} IN[];

out Vertex {
	vec4 colour;
	vec2 texCoord;
} OUT[];

void main () {
	gl_TessLevelInner[0] = 5;
	gl_TessLevelInner[1] = 5;

	gl_TessLevelOuter[0] = 6;
	gl_TessLevelOuter[1] = 6;
	gl_TessLevelOuter[2] = 6;
	gl_TessLevelOuter[3] = 6;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}