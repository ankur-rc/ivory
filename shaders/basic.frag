#version 330 core
out vec4 FragColor;
uniform vec4 color = vec4(1.f, 0.5f, 0.2f, 1.f);

void main() { FragColor = color; }