#version 150

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3 cameraPos;
uniform vec4 lightColour[4];
uniform vec3 lightPos[4];
uniform float lightRadius[4];

uniform float time;

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
	for (int i = 0; i < 4; i++)
	{
		vec4 diffuse = texture(diffuseTex, IN.texCoord);
		mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
		vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

		vec3 incident = normalize(lightPos[i] - IN.worldPos);
		float lambert = max(0.0, dot(incident, normal));
		float dist = length(lightPos[i] - IN.worldPos);
		float atten = 1.0 - clamp(dist / lightRadius[i], 0.0, 1.0);
		vec3 viewDir = normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);
	
		float rFactor = max(0.0, dot(halfDir, normal));
		float sFactor = pow(rFactor, 33.0);
		vec3 colour = (diffuse.rgb * lightColour[i].rgb);
		colour += (lightColour[i].rgb * sFactor) * 0.33;
	
		vec4 col = vec4(colour * atten * lambert, diffuse.a);
		col.rgb += (diffuse.rgb * lightColour[i].rgb) * 0.1;

		fragColour += col;
	}

	//fragColour = fragColour / 4;
}