#version 150 core

uniform sampler2D diffuseTex;
uniform vec2 pixelSize;
uniform int isVertical;

in Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

const float weights[5] = float[](0.12, 0.22, 0.32, 0.22, 0.12);

void main(void)	{
	vec2 values[5];
	fragColour = vec4(0, 0, 0, 0);
	
	if (isVertical == 1)
	{
		values = vec2[](vec2(0.0, -pixelSize.y * 3),
						vec2(0.0, -pixelSize.y * 2),
						vec2(0.0, pixelSize.y),
						vec2(0.0, pixelSize.y * 2),
						vec2(0.0, pixelSize.y * 3));
	}
	else
	{
		values = vec2[](vec2(-pixelSize.x * 3, 0.0),
						vec2(-pixelSize.x * 2, 0.0),
						vec2(pixelSize.x, 0.0),
						vec2(pixelSize.x * 2, 0.0),
						vec2(pixelSize.x * 3, 0.0));
	}
	
	for (int i = 0; i < 5; i++)
	{
		vec4 tmp = texture2D(diffuseTex, IN.texCoord.xy + values[i]);
		fragColour += tmp * weights[i];
	}
}