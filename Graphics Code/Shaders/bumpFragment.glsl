#version 150

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3 cameraPos;
uniform vec4 lightColour[4];
uniform vec3 lightPos[4];
uniform float lightRadius[4];

in Vertex {
	vec3 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
	vec4 diffuse[4];
	mat3 TBN[4];
	vec3 normal[4];

	vec3 incident[4];
	float lambert[4];
	float dist[4];
	float atten[4];
	vec3 viewDir[4];
	vec3 halfDir[4];

	float rFactor[4];
	float sFactor[4];
	vec3 colour[4];
	vec4 col[4];

	for (int i = 0; i < 4; i++)
	{
		diffuse[i] = texture(diffuseTex, IN.texCoord);
		TBN[i] = mat3(IN.tangent, IN.binormal, IN.normal);
		normal[i] = normalize(TBN[i] * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

		incident[i] = normalize(lightPos[i] - IN.worldPos);
		lambert[i] = max(0.0, dot(incident[i], normal[i]));
		dist[i] = length(lightPos[i] - IN.worldPos);
		atten[i] = 1.0 - clamp(dist[i] / lightRadius[i], 0.0, 1.0);
		viewDir[i] = normalize(cameraPos - IN.worldPos);
		halfDir[i] = normalize(incident[i] + viewDir[i]);
	
		rFactor[i] = max(0.0, dot(halfDir[i], normal[i]));
		sFactor[i] = pow(rFactor[i], 33.0);
		colour[i] = (diffuse[i].rgb * lightColour[i].rgb);
		colour[i] += (lightColour[i].rgb * sFactor[i]) * 0.33;
	
		col[i] = vec4(colour[i] * atten[i] * lambert[i], diffuse[i].a);
		col[i].rgb += (diffuse[i].rgb * lightColour[i].rgb) * 0.1;
		fragColour += col[i];
	}

	//fragColour = fragColour / 4;
}