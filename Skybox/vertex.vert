#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 Tangent;
out vec3 Bitangent;

uniform vec2 Offset;
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position.x + Offset.x, position.y + Offset.y, position.z, 1.0);
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(position, 1.0));
    Tangent = length(tangent) > 0.0 ? tangent : vec3(1.0, 0.0, 0.0);
	Bitangent = length(bitangent) > 0.0 ? bitangent : vec3(0.0, 1.0, 0.0);
}