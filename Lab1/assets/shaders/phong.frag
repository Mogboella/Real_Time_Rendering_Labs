#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float shininess;
uniform float ks; // Specular coefficient

void main()
{
    float spec = 0.0;
    vec3 ambient = 0.05 * lightColor;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    if (diff > 0.0)
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    
    vec3 specular = (ks * spec) * lightColor;
    
    vec3 result = (ambient + diffuse) * objectColor + specular;
    color = vec4(result, 1.0);
}
