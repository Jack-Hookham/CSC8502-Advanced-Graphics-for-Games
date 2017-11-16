#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform samplerCube shadowTex;
uniform int useTexture;
uniform float sphereRadius;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform mat4 projMatrix;

in Vertex
{
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
	vec4 	shadowProj;
} IN;

out vec4 fragColour;

void main(void)
{
	// fragColour = IN.colour;
	// if (useTexture > 0)
	// {
	// 	fragColour = texture(diffuseTex, IN.texCoord);
	// }

	//fragColour.xy = IN.texCoord;


	//------------Triplanar texture mapping----------------------------------------------
	//https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821

	vec3 worldPosScaled = IN.worldPos / 100;
	vec3 wNorm = normalize(IN.normal);
	vec3 blending = abs(wNorm);
	blending = normalize(max(blending, 0.00001));
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);

	vec4 xaxis = texture2D(diffuseTex, worldPosScaled.yz);
	vec4 yaxis = texture2D(diffuseTex, worldPosScaled.xz);
	vec4 zaxis = texture2D(diffuseTex, worldPosScaled.xy);

	vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

	fragColour = tex;

	//-----------Add Lighting-------------------------------------------------------

	//vec4 diffuse = texture(diffuseTex, IN.texCoord);
	
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec4 diffuse = (texture(diffuseTex, IN.texCoord) * clamp(dot(IN.normal, incident), 0.0, 1.0));

	float lambert = max(0.0, dot(incident, IN.normal));
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float rFactor = max(0.0, dot(halfDir, IN.normal));
	float sFactor = pow(rFactor, 32.0);

	//--------Shadow Stuff----------------------------------------------------------
	float shadow = 1.0;
	
	//Inverse incident so that it's taken from the light perspective rather than the object
	vec3 invIncident = -incident;

	//Linearize depth value 
	//http://www.ozone3d.net/blogs/lab/20090206/how-to-linearize-the-depth-value/
	//linearize(texture(shadowTex, incident).r);

	float n = 1.0;
	float f = 1000000.0;
	float z_b = texture(shadowTex, invIncident).x;
    float z_n = 2.0 * z_b - 1.0;
	float shadowDist = 2.0 * n * f / (f + n - z_n * (f - n));

   	float A = projMatrix[2].z;
    float B = projMatrix[3].z;
	dist = length(lightPos - IN.worldPos + wNorm * 10.0);
	dist -= 80.0;
    dist  = 0.5 * (-A * dist + B) / dist + 0.5;

	shadowDist = texture(shadowTex, invIncident).x;
	if (dist > shadowDist)
	{
		shadow = 0.0;
	}

	//shadow = clamp((shadowDist - dist), 0, 1);
	float test = dist - shadowDist;

	//fragColour = vec4(test / 1000.0f,test / 1000.0f,test / 1000.0f,1);

	//shadowDist = max(dist - shadowDist, 0);

	//fragColour = vec4(shadowDist/100.0, shadowDist/1000.0, shadowDist/10000.0,1);
	//return;

	//fragColour = vec4(vec3(shadow), 1);
	//return;

	lambert *= shadow;

	// vec4 csDepth = (projMatrix * vec4(0, 0, dist, 1));
	// 	float depth = csDepth.z / csDepth.w;

	// 	vec4 samplerId = vec4(0);
	// 	samplerId.xyw = incident;
	// 	samplerId.z = depth;
		
	// 	shadow = texture(shadowTex, samplerId);
	// 	fragColour = vec4(vec3(shadow), 1);
	// 	return;


	vec3 colour = (diffuse.rgb * lightColour.rgb);
	colour += (lightColour.rgb * sFactor) * 0.33;
	
	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

	fragColour *= vec4((colour * atten * lambert), diffuse.a) + ambient;

	//fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;

	// vec4 diffuse = texture(diffuseTex, IN.texCoord);
	// diffuse.rgb *= vec3(0.5, 0.42, 0.37);
	// mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	// vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

	// vec3 incident = normalize(lightPos - IN.worldPos);
	// float lambert = max(0.0, dot(incident, normal));
	// float dist = length(lightPos - IN.worldPos);
	// float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	// vec3 viewDir = normalize(cameraPos - IN.worldPos);
	// vec3 halfDir = normalize(incident + viewDir);
	
	// float rFactor = max(0.0, dot(halfDir, normal));
	// float sFactor = pow(rFactor, 32.0);
	// vec3 colour = (diffuse.rgb * lightColour.rgb);
	// colour += (lightColour.rgb * sFactor) * 0.2;
	
	// vec4 col = vec4(colour * atten * lambert, diffuse.a);
	// col.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;

	// fragColour *= col;

	//fragColour = diffuse;

	//fragColour.rgb = vec3(blending);
}
