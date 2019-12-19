#version 330 core

in vec2 out_textureCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;

uniform vec3 viewPos;

out vec4 outColour;

void main()
{
    vec3 FragPos = texture(gPosition, out_textureCoords).rgb;
    vec3 Normal = texture(gNormal, out_textureCoords).rgb;
    vec3 Albedo = texture(gColour, out_textureCoords).rgb;
    float specular = texture(gPosition, out_textureCoords).a;

    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // diffuse
    vec3 lightDir = normalize(vec3(10, 10, 10) - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * vec3(1,1,1);
    lighting += diffuse;
    
    outColour = vec4(lighting, 1.0);

} 