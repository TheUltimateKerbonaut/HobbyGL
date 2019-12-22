#version 330 core

in vec2 out_textureCoords;

uniform sampler2D hdrTexture;

uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 outColour;

void main()
{
	vec2 tex_offset = 1.0 / textureSize(hdrTexture, 0); // gets size of single texel
	vec3 result = texture(hdrTexture, out_textureCoords).rgb * weight[0]; // current fragment's contribution
	if (horizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(hdrTexture, out_textureCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(hdrTexture, out_textureCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(hdrTexture, out_textureCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(hdrTexture, out_textureCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	outColour = vec4(result, 1.0);
} 