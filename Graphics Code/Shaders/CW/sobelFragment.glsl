#version 330 core

uniform sampler2D diffuseTex;
uniform vec2 pixelSize;

vec2 textureCoordinate;
vec2 leftTextureCoordinate;
vec2 rightTextureCoordinate;

vec2 topTextureCoordinate;
vec2 topLeftTextureCoordinate;
vec2 topRightTextureCoordinate;

vec2 bottomTextureCoordinate;
vec2 bottomLeftTextureCoordinate;
vec2 bottomRightTextureCoordinate;

in Vertex {
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

vec3 W = vec3(0.2125, 0.7154, 0.0721);

void main(void)
{
	float luminance = dot(texture2D(diffuseTex, IN.texCoord).rgb, W);

	fragColour = vec4(vec3(luminance), 1.0);

	float imageWidthFactor = pixelSize.x;
	float imageHeightFactor = pixelSize.y;

	vec2 widthStep = vec2(imageWidthFactor, 0.0);
    vec2 heightStep = vec2(0.0, imageHeightFactor);
	vec2 widthHeightStep = vec2(imageWidthFactor, imageHeightFactor);
	vec2 widthNegativeHeightStep = vec2(imageWidthFactor, -imageHeightFactor);

	textureCoordinate = IN.texCoord.xy;
    leftTextureCoordinate = IN.texCoord.xy - widthStep;
    rightTextureCoordinate = IN.texCoord.xy + widthStep;

    topTextureCoordinate = IN.texCoord.xy + heightStep;
    topLeftTextureCoordinate = IN.texCoord.xy - widthNegativeHeightStep;
    topRightTextureCoordinate = IN.texCoord.xy + widthHeightStep;

    bottomTextureCoordinate = IN.texCoord.xy - heightStep;
    bottomLeftTextureCoordinate = IN.texCoord.xy - widthHeightStep;
    bottomRightTextureCoordinate = IN.texCoord.xy + widthNegativeHeightStep;

	float i00   = texture2D(diffuseTex, textureCoordinate).r;
    float im1m1 = texture2D(diffuseTex, bottomLeftTextureCoordinate).r;
    float ip1p1 = texture2D(diffuseTex, topRightTextureCoordinate).r;
    float im1p1 = texture2D(diffuseTex, topLeftTextureCoordinate).r;
    float ip1m1 = texture2D(diffuseTex, bottomRightTextureCoordinate).r;
    float im10 = texture2D(diffuseTex, leftTextureCoordinate).r;
    float ip10 = texture2D(diffuseTex, rightTextureCoordinate).r;
    float i0m1 = texture2D(diffuseTex, bottomTextureCoordinate).r;
    float i0p1 = texture2D(diffuseTex, topTextureCoordinate).r;
    float h = -im1p1 - 2.0 * i0p1 - ip1p1 + im1m1 + 2.0 * i0m1 + ip1m1;
    float v = -im1m1 - 2.0 * im10 - im1p1 + ip1m1 + 2.0 * ip10 + ip1p1;

    float mag = length(vec2(h, v));

	fragColour *= vec4(vec3(mag), 1.0);
}