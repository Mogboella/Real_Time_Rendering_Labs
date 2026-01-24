#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float roughness;

void main()
{

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    float NdotL = clamp(dot(normal, lightDir), 0.0, 1.0);
    float NdotV = clamp(dot(normal, viewDir), 0.0, 1.0);

    vec3 ambient = 0.1 * lightColor;

    if (NdotL <= 0.0)
    {
        vec3 result = ambient * objectColor;
        color = vec4(result, 1.0);
        return;
    }

    vec3 angleVN = normalize(viewDir - normal * NdotV);
    vec3 angleLN = normalize(lightDir - normal * NdotL);
    float gamma = max(0.0, dot(angleVN, angleLN));

    float thetaI = acos(NdotL);
    float thetaR = acos(NdotV);

    float alpha = max(thetaI, thetaR);
    float beta = min(thetaI, thetaR);

    float sigma2 = roughness * roughness;

    float A = 1.0 - 0.5 * (sigma2 / (sigma2 + 0.57));
    float B = 0.45 * (sigma2 / (sigma2 + 0.09));
    float C = sin(alpha) * tan(beta);

    float OrenNayar = NdotL * (A + B * gamma * C);
    vec3 diffuse = OrenNayar * lightColor;
    
    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0);
}

