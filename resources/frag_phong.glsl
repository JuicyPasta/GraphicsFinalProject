#version 330 core 
in vec3 fragNor;
out vec4 color;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shiny;
uniform vec3 ambient;

in vec3 L;
in vec3 H;

void main() {
    vec3 sourceColor = vec3(1, 1, 1);

    vec3 L_norm = normalize(L);
    vec3 H_norm = normalize(H);
    vec3 fragNor_norm = normalize(fragNor);

    vec3 diffuse_part = diffuse * max(dot(fragNor_norm, L_norm), 0.0) * sourceColor;
    vec3 specular_part = specular * pow(max(dot(fragNor_norm, H_norm), 0.0), shiny) * sourceColor;

    color = vec4(diffuse_part + specular_part + ambient, 1);
}
