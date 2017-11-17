#version 400 core

//tessellation control for destroying the cube

layout (triangles, fractional_even_spacing, ccw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
} IN[];

out Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
} OUT;

vec2 CombineVec2(vec2 a, vec2 b, vec2 c)
 {
    vec2 p0 = gl_TessCoord.x * a;
    vec2 p1 = gl_TessCoord.y * b;
    vec2 p2 = gl_TessCoord.z * c;
    return p0 + p1 + p2;
}

vec3 CombineVec3(vec3 a, vec3 b, vec3 c)
 {
    vec3 p0 = gl_TessCoord.x * a;
    vec3 p1 = gl_TessCoord.y * b;
    vec3 p2 = gl_TessCoord.z * c;
    return p0 + p1 + p2;
}

vec4 CombineVec4(vec4 a, vec4 b, vec4 c) 
{
    vec4 p0 = gl_TessCoord.x * a;
    vec4 p1 = gl_TessCoord.y * b;
    vec4 p2 = gl_TessCoord.z * c;
    return p0 + p1 + p2;
}

void main () {
	vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

	vec3 combinedPos = p0 + p1 + p2;

	vec4 worldPos = modelMatrix * vec4 ( combinedPos , 1);

	gl_Position = projMatrix * viewMatrix * worldPos;

	OUT.colour = CombineVec4(IN[0].colour, IN[1].colour, IN[2].colour);
	OUT.texCoord = CombineVec2(IN[0].texCoord, IN[1].texCoord, IN[2].texCoord);
	OUT.normal = CombineVec3(IN[0].normal, IN[1].normal, IN[2].normal);
	OUT.tangent = CombineVec3(IN[0].tangent, IN[1].tangent, IN[2].tangent);
	OUT.binormal = CombineVec3(IN[0].binormal, IN[1].binormal, IN[2].binormal);
	OUT.worldPos = CombineVec3(IN[0].worldPos, IN[1].worldPos, IN[2].worldPos);
}