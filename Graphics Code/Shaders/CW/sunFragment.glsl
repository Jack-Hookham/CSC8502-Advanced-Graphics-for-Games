#version 330 core
//sun
uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform int useTexture;
uniform vec4 nodeColour;

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
		//------------Triplanar texture mapping----------------------------------------------
		//https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821
		// 	fragColour = texture(diffuseTex, IN.texCoord);
		vec3 worldPosScaled = IN.worldPos / 500;
		vec3 wNorm = normalize(IN.normal);
		vec3 blending = abs(wNorm);
		blending = normalize(max(blending, 0.00001));
		float b = (blending.x + blending.y + blending.z);
		blending /= vec3(b, b, b);

		vec4 xaxis = texture2D(diffuseTex, worldPosScaled.yz);
		vec4 yaxis = texture2D(diffuseTex, worldPosScaled.xz);
		vec4 zaxis = texture2D(diffuseTex, worldPosScaled.xy);

		vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

		fragColour = tex * nodeColour.a;
	}
}
