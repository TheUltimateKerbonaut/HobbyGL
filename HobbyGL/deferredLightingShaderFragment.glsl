#version 330 core

#define MAX_LIGHTS 25

in vec2 out_textureCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;
uniform sampler2D ssao;

uniform vec3 directionalColour[MAX_LIGHTS];
uniform vec3 directionalPos[MAX_LIGHTS];
uniform int directionals;

uniform vec3 pointColour[MAX_LIGHTS];
uniform vec3 pointPos[MAX_LIGHTS];
uniform vec3 pointAttenuation[MAX_LIGHTS];
uniform float pointRange[MAX_LIGHTS];
uniform int points;

out vec4 outColour;


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

const int indexMatrix4x4[81] = int[](
	50, 71, 2, 23, 44, 56, 77, 17, 29,
	72, 12, 33, 45, 66, 6, 18, 39, 60,
	22, 43, 55, 76, 16, 28, 49, 70, 1,
	53, 65, 5, 26, 38, 59, 80, 11, 32,
	75, 15, 27, 48, 69, 0, 21, 42, 54,
	25, 37, 58, 79, 10, 31, 52, 64, 4,
	47, 68, 8, 20, 41, 62, 74, 14, 35,
	78, 9, 30, 51, 63, 3, 24, 36, 57,
	19, 40, 61, 73, 13, 34, 46, 67, 7
);

float indexValue() {
	int x = int(mod(gl_FragCoord.x, 9));
	int y = int(mod(gl_FragCoord.y, 9));
	return indexMatrix4x4[(x + y * 9)] / 81.0;
}

float dither(float color) {
	float closestColor = (color < 0.5) ? 0 : 1;
	float secondClosestColor = 1 - closestColor;
	float d = indexValue();
	float distance = abs(closestColor - color);
	return (distance < d) ? closestColor : secondClosestColor;
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
		lighting += calcDiffuse(Normal, direction, directionalColour[i], 1);
		lighting += calcSpecular(specular, viewDir, Normal, direction, directionalColour[i], 1);
	}

	// Point lights
	for (int i = 0; i < points; ++i)
	{
		lighting += calculatePointLight(i, FragPos, Normal, specular, viewDir);
	}
	
	outColour = vec4(Albedo * lighting, 1.0);
	
	if (outColour != vec4(0.0, 0.0, 0.0, 1.0))
		outColour = vec4(dither(outColour.x), dither(outColour.y), dither(outColour.z), 1.0);

}