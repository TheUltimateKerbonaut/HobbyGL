#version 330 core

in vec2 out_textureCoords;

uniform sampler2D noiseTexture;
uniform sampler2D gNormalTexture;
uniform sampler2D gPositionTexture;

uniform vec3 samples[64];
uniform mat4 projectionMatrix;
uniform vec2 noiseScale;

out float outColour;

void main()
{
	const int kernelSize = 64;
	const float radius = 0.5;
	const float bias = 0.025;


	vec3 fragPos = texture(gPositionTexture, out_textureCoords).xyz;
	vec3 normal = texture(gNormalTexture, out_textureCoords).rgb;
	vec3 randomVec = texture(noiseTexture, out_textureCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		// get sample position
		vec3 sample = TBN * samples[i]; // From tangent to view-space
		sample = fragPos + sample * radius;

		vec4 offset = vec4(sample, 1.0);
		offset = projectionMatrix * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  

		float sampleDepth = texture(gPositionTexture, offset.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	

	occlusion = 1.0 - (occlusion / kernelSize);
	outColour = pow(occlusion, 5);
} 