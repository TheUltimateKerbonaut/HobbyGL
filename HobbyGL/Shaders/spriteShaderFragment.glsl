#version 330 core

in vec2 out_textureCoords;

uniform sampler2D diffuseTexture;

out vec4 outColour;

void main()
{
	outColour = texture(diffuseTexture, out_textureCoords);
} 