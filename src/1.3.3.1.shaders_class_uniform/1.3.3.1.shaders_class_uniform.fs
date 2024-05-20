#version 330 core
out vec4 FragColor;

uniform float greenValue;

void main()
{
    FragColor = vec4(0.0, greenValue, 0.0, 1.0);
}