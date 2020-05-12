#version 330 core

in vec3 position;
in vec3 normal;
in vec2 textureCoords;
in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform bool hasNormalMap;

out vec3 out_position;
out vec3 out_normal;
out vec2 out_textureCoords;
out mat3 TBN;

uniform bool hasReflectionMap;
out vec3 out_worldPos;
out vec3 out_worldNormal;

void main()
{
	vec4 worldPos = modelMatrix * vec4(position, 1.0);
	vec4 viewPos = viewMatrix * worldPos;
	out_position = viewPos.xyz;
	out_textureCoords = textureCoords;

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
	out_normal = normalMatrix * normal;

	gl_Position = projectionMatrix * viewPos;

	if (hasNormalMap)
	{
		vec3 T = normalize(vec3(modelViewMatrix * vec4(tangent, 0.0)));
		vec3 B = normalize(vec3(modelViewMatrix * vec4(cross(normal, tangent), 0.0)));
		vec3 N = normalize(vec3(modelViewMatrix * vec4(normal, 0.0)));
		TBN = mat3(T, B, N);
	}

	if (hasReflectionMap)
	{
		out_worldPos = worldPos.xyz;
		out_worldNormal = transpose(inverse(mat3(modelMatrix))) * normal;
	}
}