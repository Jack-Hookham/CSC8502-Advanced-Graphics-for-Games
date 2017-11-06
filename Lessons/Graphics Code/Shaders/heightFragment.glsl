#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec3 position;
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;

void main(void){

	if(IN.position.y < 220)
	{
		gl_FragColor = mix(mix(texture(diffuseTex, IN.texCoord), IN.colour, 0.7), mix(texture(diffuseTex, IN.texCoord), IN.colour, 0.3), IN.position.y/257);
	}	
	else
	{
		gl_FragColor = mix(mix(texture(diffuseTex, IN.texCoord), peakColour, 0.7), mix(texture(diffuseTex, IN.texCoord), peakColour, 0.3), IN.position.y/257);
	}
	if(gl_FragColor.a <= 0)
	{
		discard;	
	}
}