#version 330 core

in vec2 out_textureCoords;

uniform sampler2D inputTexture;

out vec4 outColour;

const float scale = 0.0025f;

void main()
{
	vec3 result;

	result.r = texture(inputTexture, out_textureCoords - scale).r;
	result.g = texture(inputTexture, out_textureCoords).g;
	result.b = texture(inputTexture, out_textureCoords + scale).b;

	outColour = vec4(result, 1.0);
}