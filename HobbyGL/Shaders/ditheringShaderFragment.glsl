#version 330 core

in vec2 out_textureCoords;

uniform sampler2D inputTexture;

out vec4 outColour;

const int size = 8;
const int indexMatrix4x4[size*size] = int[](
	36, 55, 2, 21, 32, 51, 6, 17,
	56, 11, 30, 41, 60, 15, 26, 45,
	20, 39, 50, 5, 16, 35, 54, 1,
	40, 59, 14, 25, 44, 63, 10, 29,
	4, 23, 34, 53, 0, 19, 38, 49,
	24, 43, 62, 9, 28, 47, 58, 13,
	52, 7, 18, 37, 48, 3, 22, 33,
	8, 27, 46, 57, 12, 31, 42, 61
);

float indexValue() {
	int x = int(mod(gl_FragCoord.x, size));
	int y = int(mod(gl_FragCoord.y, size));
	return indexMatrix4x4[(x + y * size)] / float(size*size);
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
	vec3 result = texture(inputTexture, out_textureCoords).rgb;

	float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));

	if (result != vec3(0.0, 0.0, 0.0))
		outColour = vec4(result.r * dither(brightness), result.g * dither(brightness), result.b * dither(brightness), 1.0);
} 