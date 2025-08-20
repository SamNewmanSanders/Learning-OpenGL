#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 diffuseColor;

out vec4 FragColor;

// A struct is a way to group related unifroms
struct Light {  
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light light;
uniform vec3 viewPos;
uniform float uAlpha;

uniform vec3 specularColor;      // Have these as uniforms for now, can always change per instance like diffuseColor
uniform float Shininess;     

void main()
{
    // --- Ambient ---
    vec3 ambient = 0.2 * diffuseColor;

    // --- Diffuse ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.position); // *Directional* light not positional
    float diff = max(dot(norm, lightDir), 0.0); // Dot product translates to cosine rule for how much light surface absorbes
    vec3 diffuse = diff * diffuseColor * light.color * light.intensity;

    // --- Specular (Blinn-Phong) ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), Shininess);
    vec3 specular = spec * specularColor * light.color * light.intensity;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, uAlpha);
}
