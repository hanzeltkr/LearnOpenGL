#version 330 core

layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 texCoord; // Texture coordinate

out vec2 TexCoord; // Pass texture coordinate to fragment shader

uniform vec2 Offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y); // Set the output texture coordinate
	gl_Position = projection * view * model * vec4(position.x + Offset.x, position.y + Offset.y, position.z, 1.0);
}