#version 330 core 
out vec4 color;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shiny;
uniform vec3 ambient;

in vec3 L;
in vec3 E;
in vec3 N;

void main() {
    vec3 sourceColor = vec3(1, 1, 1);

    vec3 L_norm = normalize(L);
    vec3 N_norm = normalize(N);
    vec3 E_norm = normalize(E);
    vec3 R_norm = normalize(-reflect(L_norm,N_norm));

    vec3 diffL = diffuse * max(dot(N_norm, L_norm), 0.0) * sourceColor;
//    diffL = clamp(diffL, 0.0, 1.0);

    vec3 specL = specular * pow(max(dot(R_norm, E_norm), 0.0), shiny) * sourceColor;
//    specL = clamp(specL, 0.0, 1.0);

    color = vec4(diffL + specL + ambient, 1);
}
