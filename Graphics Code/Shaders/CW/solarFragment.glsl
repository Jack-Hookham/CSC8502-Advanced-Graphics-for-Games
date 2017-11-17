#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform samplerCube shadowTex;
//uniform samplerCubeShadow shadowTex;
uniform int useTexture;
uniform float sphereRadius;
uniform vec4 nodeColour;

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
	fragColour = nodeColour;

	//------------Triplanar texture mapping----------------------------------------------
	//https://gamedevelopment.tutsplus.com/articles/use-tri-planar-texture-mapping-for-better-terrain--gamedev-13821

	if (useTexture > 0)
	{
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
	}

	//-----------Add Lighting-------------------------------------------------------
	
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
	//https://stackoverflow.com/questions/6652253/getting-the-true-z-value-from-the-depth-buffer

	//Get the distance from light
	vec3 absVec = abs(lightPos - IN.worldPos + IN.normal);

	//Take the maximum of all 3 axes and use that as the depth
	//This gives both the side of the cube map to sample and the depth
	//Manual negative adjustment bias to reduce shadow acne
	float worldDepth = max(absVec.x, max(absVec.y, absVec.z)) - 5;

	const float n = 1.0;
	const float f = 10000.0f;
	float z_b = texture(shadowTex, invIncident).x;
	float z_n = 2.0 * z_b - 1.0;
	float shadowDist = (2.0 * n * f) / (f + n - z_n * (f - n));

	if (shadowDist < worldDepth)
	{
		shadow = 0.0;
	}

	// float ndcDepth = (f + n - (2 * f *n) / worldDepth) / (f - n);
	
	// vec4 shadowTc;
	// shadowTc.xyz = invIncident;
	// shadowTc.w = ndcDepth * 0.5f + 0.5f; //Convert to texture 0-1
	
	// shadow = texture(shadowTex, shadowTc);
	
	lambert *= shadow;
	fragColour *= lambert;

	vec3 colour = (diffuse.rgb * lightColour.rgb);
	colour += (lightColour.rgb * sFactor) * 0.33;
	
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	fragColour += ambient;

	fragColour *= vec4((colour * atten * lambert), diffuse.a) + ambient;
}
