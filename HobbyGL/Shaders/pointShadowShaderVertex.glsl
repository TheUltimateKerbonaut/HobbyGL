#version 330 core

in vec3 position;

uniform mat4 MVP;
uniform mat4 modelMatrix;

out vec4 FragPos;

void main()
{
	FragPos = modelMatrix * vec4(position, 1.0);
	gl_Position = MVP * vec4(position, 1.0);
}