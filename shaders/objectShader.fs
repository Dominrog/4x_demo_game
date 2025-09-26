#version 460 core
out vec4 FragColor;

in float Brightness;
in vec3 vertexColor;

uniform vec3 baseColor;

void main()
{
    FragColor = vec4(vertexColor * Brightness, 1.0);
}
