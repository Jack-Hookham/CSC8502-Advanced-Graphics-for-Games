#version 150

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform float time;

in Vertex
{
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
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 colour;
	float lambert;
	float atten;

	//Rock
	if (IN.worldPos.y < 500)
	{
		// fragColour = mix(mix(texture(diffuseTex, IN.texCoord), 
		// vec4(IN.colour, 1.0), 0.7), mix(texture(diffuseTex, IN.texCoord), 
		// vec4(IN.colour, 1.0), 0.3), IN.worldPos.y/257);
		diffuse.rgb *= vec3(0.5, 0.5, 0.5);

		mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
		vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

		vec3 incident = normalize(lightPos - IN.worldPos);
		lambert = max(0.0, dot(incident, normal));
		float dist = length(lightPos - IN.worldPos);
		atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
		vec3 viewDir = normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);

		float rFactor = max(0.0, dot(halfDir, normal));
		float sFactor = pow(rFactor, 33.0);
		colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.2;
	}
	//Snow - don't use bump texture
	else
	{
		diffuse.rgb = vec3(1.0, 1.0, 1.0);
		vec3 incident = normalize(lightPos - IN.worldPos);
		lambert = max(0.0, dot(incident, IN.normal));
		float dist = length(lightPos - IN.worldPos);
		atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
		vec3 viewDir = normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);

		float rFactor = max(0.0, dot(halfDir, IN.normal));
		float sFactor = pow(rFactor, 10.0);
		colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.33;
	}
	//diffuse.rgb *= vec3(0.5, 0.5, 0.5);

	fragColour = vec4(colour * atten * lambert, diffuse.a);
	fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
}