#version 330 core
in vec3 LightingColor;

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
    FragColor = vec4(LightingColor * objectColor, 1.0);
}