#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float bands; // Number of toon shading bands
uniform float minShade; // Minimum shade factor

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    float intensity = max(dot(lightDir, normal), 0.0);

    float b = max(bands, 1.0);
    float q = floor(intensity * b) / b;
    float shade = mix(minShade, 1.0, q);

    vec3 pixelColor = lightColor * objectColor * shade;
    color = vec4(pixelColor, 1.0);

}

