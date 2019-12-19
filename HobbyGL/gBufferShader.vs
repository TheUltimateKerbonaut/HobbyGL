#version 330 core

in vec3 position;
in vec3 normal;
in vec2 textureCoords;

uniform mat4 MVP;

out vec3 out_position;
out vec3 out_normal;
out vec2 out_textureCoords;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
	out_position = position;
	out_normal = normal;
	out_textureCoords = textureCoords;
}