#version 330 core

//slowly moves everything towards the center of the object
//then pauses movement
//then quickly moves everything further inwards, turning the object inside out

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

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

void main()
{
	vec4 norm = normalize(gl_in[0].gl_Position);
	gl_Position = gl_in[0].gl_Position;
	if (time <= 300)
	{
		gl_Position.x -= norm.x * time * 0.1;
		gl_Position.y -= norm.y * time * 0.1;
	}
	if (time > 300 && time <= 400)
	{
		gl_Position.x -= norm.x * 300 * 0.1;
		gl_Position.y -= norm.y * 300 * 0.1;
	}
	if (time > 360)
	{
		gl_Position.x -= norm.x * (time - 360) * 0.005 * time;
		gl_Position.y -= norm.y * (time - 360) * 0.005 * time;
	}
	OUT.colour = vec4(0.5,1.0,0.0,1.0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	if (time <= 300)
	{
		gl_Position.x -= norm.x * time * 0.1;
		gl_Position.y -= norm.y * time * 0.1;
	}
	if (time > 300 && time <= 400)
	{
		gl_Position.x -= norm.x * 300 * 0.1;
		gl_Position.y -= norm.y * 300 * 0.1;
	}
	if (time > 360)
	{
		gl_Position.x -= norm.x * (time - 360) * 0.005 * time;
		gl_Position.y -= norm.y * (time - 360) * 0.005 * time;
	}
	OUT.colour = vec4(1.0,0.0,0.5,1.0);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	if (time <= 300)
	{
		gl_Position.x -= norm.x * time * 0.1;
		gl_Position.y -= norm.y * time * 0.1;
	}
	if (time > 300 && time <= 400)
	{
		gl_Position.x -= norm.x * 300 * 0.1;
		gl_Position.y -= norm.y * 300 * 0.1;
	}
	if (time > 360)
	{
		gl_Position.x -= norm.x * (time - 360) * 0.005 * time;
		gl_Position.y -= norm.y * (time - 360) * 0.005 * time;
	}
	OUT.colour = vec4(0.0,0.5,1.0,1.0);
	EmitVertex();

	EndPrimitive();

}

