#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

in vec3 out_position;
in vec3 out_normal;
in vec2 out_textureCoords;
in mat3 TBN;

uniform bool hasNormalMap;
uniform sampler2D normalMap;

uniform bool hasSpecularMap;
uniform sampler2D specularMap;

uniform sampler2D diffuseTexture;
uniform float specularFactor;

uniform float textureTiling;

void main()
{
    gPosition = out_position;
    gAlbedoSpec.rgb = texture(diffuseTexture, textureTiling * out_textureCoords).rgb;
	gNormal.a = specularFactor;

	gNormal.rgb = normalize(out_normal);
	
	if (hasNormalMap)
	{
		vec3 normal = texture(normalMap, textureTiling * out_textureCoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		gNormal.rgb = normalize(TBN * normal);
	}

	if (hasSpecularMap)
	{
		gNormal.a *= texture(specularMap, textureTiling * out_textureCoords).r;
	}
}