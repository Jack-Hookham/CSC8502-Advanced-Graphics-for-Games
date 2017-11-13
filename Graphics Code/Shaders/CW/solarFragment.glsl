#version 150 core

uniform sampler2D diffuseTex;
uniform int useTexture;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex
{
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
	fragColour = IN.colour;
	if (useTexture > 0)
	{
		fragColour *= texture(diffuseTex, IN.texCoord);
	}

	//fragColour.xy = IN.texCoord;
}