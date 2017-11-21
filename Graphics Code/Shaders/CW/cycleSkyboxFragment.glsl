#version 150 core

uniform samplerCube cubeTex;
uniform samplerCube shadowTex;
uniform vec3 cameraPos;
uniform float sunStrength;

in Vertex {
	vec3 normal;
} IN;

out vec4 fragColour;

void main(void)	
{
	fragColour = texture(cubeTex, normalize(IN.normal));

	float lightMod = sunStrength;
	if (lightMod < 0.1)
	{
		lightMod = 0.1;
	}
	fragColour *= lightMod;
}