#version 330 core

in vec3 position;
in vec3 normal;
in vec2 textureCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 out_position;
out vec3 out_normal;
out vec2 out_textureCoords;

void main()
{
	vec4 viewPos = viewMatrix * modelMatrix * vec4(position, 1.0);
	out_position = viewPos.xyz;
	out_textureCoords = textureCoords;

	mat3 normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
	out_normal = normalMatrix * normal;

	gl_Position = projectionMatrix * viewPos;
}