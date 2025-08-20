#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// Per-instance attributes
layout(location = 2) in mat4 instanceModel;   // Compiler knows to combine locations 2-5 into a mat4
layout(location = 6) in vec3 instanceDiffuseColor;   // occupies location 6

out vec3 FragPos;
out vec3 Normal;
out vec3 diffuseColor; 

uniform mat4 view;
uniform mat4 projection;
uniform vec3 uniformDiffuseColor; // For non - instanced draw calls

uniform mat4 model;          // For non - instanced draw calls
uniform bool useInstancing;  // So the shader know if instanced or not

void main()
{
    // Retrieve the correct model matrix
    mat4 finalModel = useInstancing ? instanceModel : model;

    // Compute world position of vertex
    FragPos = vec3(finalModel * vec4(aPos, 1.0));
    
    // Transform normal by instance model matrix
    Normal = mat3(transpose(inverse(finalModel))) * aNormal;

    // Pass per-instance color to fragment shader using the correct vec3 case
    diffuseColor = useInstancing ? instanceDiffuseColor : uniformDiffuseColor;

    // Compute clip-space position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}







