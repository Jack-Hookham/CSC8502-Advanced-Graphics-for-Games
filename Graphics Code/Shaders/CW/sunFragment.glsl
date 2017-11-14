#version 150 core
//sun
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
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
	vec3 temp = IN.worldPos / 300;
	vec3 wNorm = normalize(IN.normal);
	vec3 blending = abs(wNorm);
	blending = normalize(max(blending, 0.00001));
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);

	vec4 xaxis = texture2D(diffuseTex, temp.yz);
	vec4 yaxis = texture2D(diffuseTex, temp.xz);
	vec4 zaxis = texture2D(diffuseTex, temp.xy);

	vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

	fragColour = tex;
}
