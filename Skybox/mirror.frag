#version 330 core

 in vec3 Normal;
 in vec3 FragPos;

 out vec4 FragColor;

 uniform vec3 cameraPos;
 uniform samplerCube skybox;

 void main() {
    // Air : 1.00, Water: 1.33, Ice: 1.309, Glass: 1.52, Diamond: 2.42
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}