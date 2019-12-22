#version 330 core

in vec3 position;
in vec2 textureCoords;

out vec2 out_textureCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
	out_textureCoords = textureCoords;
}