#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 out_position;
in vec3 out_normal;
in vec2 out_textureCoords;

uniform sampler2D diffuseTexture;
uniform float specularFactor;

void main()
{
    gPosition = out_position;
    gNormal = normalize(out_normal);
    gAlbedoSpec.rgb = texture(diffuseTexture, out_textureCoords).rgb;
    gAlbedoSpec.a = specularFactor;
} 