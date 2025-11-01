#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D screenTexture;

vec4 postProcess();
vec4 kernelEffect();

const float offset = 1.0 / 300.0; 

void main() {
	color = kernelEffect();
}

vec4 postProcess() {
	color = texture(screenTexture, TexCoord);

	// Invert the colors
	//color = vec4(vec3(1.0 - texture(screenTexture, TexCoord)), 1.0);

	// Grayscale effect
	float average = (color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722) / 3.0;
	color = vec4(average, average, average, 1.0);
	return color;
};

vec4 kernelEffect() {
	// Setting offset
	vec2 offsets[9] = vec2[](
	vec2(-offset, offset),
	vec2(0, offset),
	vec2(offset, offset),
	vec2(-offset, 0),
	vec2(0, 0),
	vec2(offset, 0),
	vec2(-offset, -offset),
	vec2(0, -offset),
	vec2(offset, -offset)
	);

	float kernel[9] = float[](
		3 , 3, 3,
		3, -15, 3,
		3, 3, 3
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(screenTexture, TexCoord.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	color = vec4(col, 1.0);
	return color;
};