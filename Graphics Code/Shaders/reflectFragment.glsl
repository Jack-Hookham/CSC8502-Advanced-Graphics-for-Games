#version 150

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform sampler2D bumpTex;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3	binormal;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

void main(void)
{
//		vec4 diffuse = vec4(0.2, 0.5, 1.0, 1.0);// * texture(diffuseTex, IN.texCoord);
//		mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
//		vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

//		vec3 incident = normalize(lightPos - IN.worldPos);
//		float lambert = max(0.0, dot(incident, normal));
//		float dist = length(lightPos - IN.worldPos);
//		float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
//		vec3 viewDir = normalize(cameraPos - IN.worldPos);
//		vec3 halfDir = normalize(incident + viewDir);
	
//		float rFactor = max(0.0, dot(halfDir, normal));
//		float sFactor = pow(rFactor, 33.0);
//		vec3 colour = (diffuse.rgb * lightColour.rgb);
//		colour += (lightColour.rgb * sFactor) * 0.33;
	
//		vec4 col = vec4(colour * atten * lambert, diffuse.a);
//		col.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;

//		fragColour += col;

	vec4 diffuse = /*vec4(0.3, 0.7, 0.9, 1.0) */ texture(diffuseTex, IN.texCoord) * IN.colour;

	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

	vec3 bumpIncident = normalize(lightPos - IN.worldPos);
	float lambert = max(0.0, dot(bumpIncident, normal));

	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(bumpIncident + viewDir);
	
	float rFactor = max(0.0, dot(halfDir, normal));
	float sFactor = pow(rFactor, 32.0);

	vec3 colour = (diffuse.rgb * lightColour.rgb);
	colour += lightColour.rgb * sFactor * 1.0;
	colour *= atten * lambert;
	colour.rgb += (diffuse.rgb * lightColour.rgb) * 0.2;
	vec4 bumpColour = vec4(colour, diffuse.a * 0.7);

	vec3 reflectIncident = normalize(IN.worldPos - cameraPos);
	vec4 reflection = texture(cubeTex, reflect(reflectIncident, normalize(IN.normal)));
	//fragColour = bumpColour;
	//fragColour = mix((lightColour * diffuse * atten) * (diffuse + reflection), (bumpColour + sFactor), 0.5);
	
	//fragColour = mix(lightColour * atten * reflection, mix(bumpColour, vec4((lightColour.rgb + sFactor), 1.0), 0.5), 0.5);
	fragColour = mix(lightColour * atten * reflection, bumpColour, 0.5);
	//fragColour.a *= 0.7;

	//	fragColour = diffuse;
	//	fragColour.xy = IN.texCoord;
}