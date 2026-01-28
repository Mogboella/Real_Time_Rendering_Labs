#version 330 core

in vec3 textureDir;
in vec3 vertexColor;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    vec4 texColor = texture(skybox, textureDir);
    FragColor = texColor * vec4(vertexColor, 1.0);
}
