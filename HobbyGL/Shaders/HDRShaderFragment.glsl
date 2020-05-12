#version 330 core

in vec2 out_textureCoords;

uniform sampler2D hdrTexture;
uniform sampler2D bloomTexture;

out vec4 outColour;

// ACES Film tonemapping algorithm
vec3 ACESFilm(vec3 x)
{
	const float a = 2.51f;
	const float b = 0.03f;
	const float c = 2.43f;
	const float d = 0.59f;
	const float e = 0.14f;
	return clamp(((x*(a*x + b)) / (x*(c*x + d) + e)), 0.0, 1.0);
}

void main()
{
	vec3 hdrColour = texture(hdrTexture, out_textureCoords).rgb + texture(bloomTexture, out_textureCoords).rgb;

	// reinhard tone mapping
	vec3 mapped = ACESFilm(hdrColour);

	// gamma correction 
	//mapped = pow(mapped, vec3(1.0 / gamma));


	outColour = vec4(mapped, 1.0);
} 