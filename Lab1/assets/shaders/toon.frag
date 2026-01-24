#version 330 core

in vec3 FragPos;
in vec3 Normal;


out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    float intensity = dot(lightDir, normal);
    intensity = max(intensity, 0.0);

    vec3 pixelColor;

    if (intensity > 0.95)
        pixelColor = lightColor * objectColor;
    else if (intensity > 0.5)
        pixelColor = lightColor * objectColor * 0.7;
    else if (intensity > 0.25)
        pixelColor = lightColor * objectColor * 0.4;
    else
        pixelColor = lightColor * objectColor * 0.1;
    
    color = vec4(pixelColor, 1.0);
}

