#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec3 position;
	vec4 colour;
} IN;

out vec4 fragColour;

vec4 peakColour = vec4(0.9, 0.9, 0.9, 0.9);

void main(void)
{
	if(IN.position.y < 150)
	{
		fragColour = mix(mix(texture(diffuseTex, IN.texCoord), IN.colour, 0.7), mix(texture(diffuseTex, IN.texCoord), IN.colour, 0.3), IN.position.y/257);
	}	
	else
	{
		fragColour = mix(mix(texture(diffuseTex, IN.texCoord), peakColour, 0.7), mix(texture(diffuseTex, IN.texCoord), peakColour, 0.3), IN.position.y/257);
	}
	if(fragColour.a <= 0)
	{
		discard;	
	}
}