#version 330 core

in vec2 out_textureCoords;

uniform sampler2D depthTexture;
uniform sampler2D blurTexture;
uniform sampler2D normalTexture;

uniform float zNear;
uniform float zFar;

uniform float focusDistance;
uniform float focusRange;
uniform float apeture;
uniform float focalLength;

out vec4 outColour;

float getDistanceFromCamera(float depth) {

	//float z = depth * 2.0 - 1.0;
	//return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
	return (2 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

float getDepthStrength()
{

	float distance = texture(depthTexture, out_textureCoords).r;

	float objectDistance = (2 * zNear) / (zFar + zNear - distance * (zFar - zNear));

	float blurAmount = abs(apeture * (focalLength * (objectDistance - focusDistance)) / (objectDistance * (focusDistance - focalLength)));
	// Equation derived from https://developer.nvidia.com/gpugems/gpugems/part-iv-image-processing/chapter-23-depth-field-survey-techniques
	return clamp(blurAmount, 0, 1);
}

void main()
{
	float dofStrength = getDepthStrength();
	
	vec3 colour = mix(texture(normalTexture, out_textureCoords).rgb, texture(blurTexture, out_textureCoords).rgb, 1-dofStrength);

	outColour = vec4(colour, 1.0);
} 