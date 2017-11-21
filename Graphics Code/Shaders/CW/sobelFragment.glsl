#version 330 core

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

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

void main(void)
{
    vec3 diffuse = texture(diffuseTex, IN.texCoord.st).rgb;
	 fragColour = vec4(diffuse, 1.0);
	 return;

    mat3 I;
    for (int i = 0; i < 3; i++) 
	{
        for (int j = 0; j < 3; j++) 
		{
            vec3 sample = texelFetch(diffuseTex, ivec2(gl_FragCoord) + ivec2(i - 1, j - 1), 0 ).rgb;
            I[i][j] = length(sample); 
    	}
	}

	float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
	float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

	float g = sqrt(pow(gx, 2.0) + pow(gy, 2.0));
	g = smoothstep(0.9, 0.9, g);
	
	fragColour = vec4(diffuse - vec3(g), 1.0);
} 
