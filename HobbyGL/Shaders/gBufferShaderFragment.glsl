#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 out_position;
in vec3 out_normal;
in vec2 out_textureCoords;
in mat3 TBN;

uniform bool hasNormalMap;
uniform sampler2D normalMap;

uniform sampler2D diffuseTexture;
uniform float specularFactor;

void main()
{
    gPosition = out_position;
    gAlbedoSpec.rgb = texture(diffuseTexture, out_textureCoords).rgb;
    gAlbedoSpec.a = specularFactor;

	gNormal = normalize(out_normal);
	
	if (hasNormalMap)
	{
		vec3 normal = texture(normalMap, out_textureCoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		gNormal = normalize(TBN * normal);
	}
}