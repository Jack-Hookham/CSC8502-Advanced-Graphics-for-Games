#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
	fragColour = texture(diffuseTex, IN.texCoord);

	//fragColour = vec4(1, 0, 0, 1);


	///fragColour.xy = IN.texCoord;
	//fragColour.a = 1;
}