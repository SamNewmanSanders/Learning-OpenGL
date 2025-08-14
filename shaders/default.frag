#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform float uAlpha;

void main() {
    // Ambient
    vec3 ambient = 0.2 * material.diffuse;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.position); // *Directional* light not positional
    float diff = max(dot(norm, lightDir), 0.0); // Dot product translates to cosine rule for how much light surface absorbes
    vec3 diffuse = diff * material.diffuse * light.color * light.intensity;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * light.color * light.intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, uAlpha);
}
