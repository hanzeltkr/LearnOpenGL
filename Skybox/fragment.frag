#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;

void main() {
    vec4 FragColor = texture(texture1, TexCoord);
    //if (FragColor.a < 0.1)
    //    discard;
    color = FragColor;
}