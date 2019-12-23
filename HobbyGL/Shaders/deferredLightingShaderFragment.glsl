#version 330 core

#define MAX_LIGHTS 25

in vec2 out_textureCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;
uniform sampler2D ssao;

uniform vec3 directionalColour[MAX_LIGHTS];
uniform vec3 directionalPos[MAX_LIGHTS];
uniform int directionalShadows[MAX_LIGHTS];
uniform mat4 directionalLightSpaceMatrices[MAX_LIGHTS];
uniform sampler2DArray directionalShadowmaps;
uniform int directionals;

uniform vec3 pointColour[MAX_LIGHTS];
uniform vec3 pointPos[MAX_LIGHTS];
uniform vec3 pointAttenuation[MAX_LIGHTS];
uniform float pointRange[MAX_LIGHTS];
uniform int points;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 brightColour;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection, int index)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(directionalShadowmaps, vec3(projCoords.xy, index)).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow

	float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);
	bias = clamp(bias, 0, 0.001);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowmaps, 0).xy;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowmaps, vec3(projCoords.xy + vec2(x, y) * texelSize, index)).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}


vec3 calcDiffuse(vec3 surfaceNormal, vec3 toLightVector, vec3 colour, float attFactor)
{
	float nDot1 = dot(surfaceNormal, toLightVector);
	float brightness = max(nDot1, 0.0);
	return (brightness * colour) * attFactor;
}

vec3 calcSpecular(float specularStrength, vec3 viewDir, vec3 normal, vec3 lightDir, vec3 lightColour, float attFactor)
{
	vec3 specular = vec3(0.0, 0.0, 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	specular = (specularStrength * spec * lightColour) * attFactor;

	return specular;
}

vec3 calculatePointLight(int i, vec3 FragPos, vec3 Normal, float specularReflectivity, vec3 viewDir)
{
	vec3 finalLighting = vec3(0, 0, 0);
	vec3 pointLightDir = pointPos[i] - FragPos;

	float distance = length(pointLightDir);

	if (distance > pointRange[i])
		return finalLighting;

	//float attFactor = pointAttenuation[i].x + (pointAttenuation[i].y * distance) + (pointAttenuation[i].z * distance * distance);
	float attFactor = 1.0 / (pointAttenuation[i].x + pointAttenuation[i].y * distance + pointAttenuation[i].z * distance * distance);

	vec3 normalisedLightDir = normalize(pointLightDir);

	finalLighting += calcDiffuse(Normal, normalisedLightDir, pointColour[i], attFactor);
	finalLighting += calcSpecular(specularReflectivity, viewDir, Normal, normalisedLightDir, pointColour[i], attFactor);

	return finalLighting;
}

void main()
{
    vec3 FragPos = texture(gPosition, out_textureCoords).rgb;
    vec3 Normal = texture(gNormal, out_textureCoords).rgb;
    vec3 Albedo = texture(gColour, out_textureCoords).rgb;
    float specular = texture(gColour, out_textureCoords).a;
	float AmbientOcclusion = texture(ssao, out_textureCoords).r;

    // Initial lighting calculations
	vec3 ambient = vec3(0.1f * AmbientOcclusion);
    vec3 lighting = ambient;
    vec3 viewDir = normalize(-FragPos);

    // Directional lights
	for (int i = 0; i < directionals; ++i)
	{
		vec3 direction = normalize(directionalPos[i] - FragPos);
		
		float shadow = 1;
		if (directionalShadows[i] >= 0) shadow -= ShadowCalculation(directionalLightSpaceMatrices[i] * vec4(FragPos, 1.0), Normal, direction, directionalShadows[i]);

		lighting += calcDiffuse(Normal, direction, directionalColour[i], 1) * shadow;
		lighting += calcSpecular(specular, viewDir, Normal, direction, directionalColour[i], 1) * shadow;
	}

	// Point lights
	for (int i = 0; i < points; ++i)
	{
		lighting += calculatePointLight(i, FragPos, Normal, specular, viewDir);
	}
	
	outColour = vec4(Albedo * lighting, 1.0);


	// Bloom
	float brightness = dot(outColour.rgb, vec3(0.2126, 0.7152, 0.0722));
	//if (brightness > 1.0)
	//	brightColour = vec4(outColour.rgb, 1.0);
	//else
	//	brightColour = vec4(0.0, 0.0, 0.0, 1.0);
	brightColour = vec4(outColour.rgb * pow(brightness, 2), 1.0);
}