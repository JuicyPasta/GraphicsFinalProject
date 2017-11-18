#version 330 core
uniform sampler2D Texture0;

out vec4 Outcolor;

in vec2 vTexCoord;
in vec3 L;
in vec3 E;
in vec3 N;

void main() {
  vec4 texColor0 = texture(Texture0, vTexCoord);

  float shiny = 200;
  float ambient = .3;

  vec3 L_norm = normalize(L);
  vec3 N_norm = normalize(N);
  vec3 E_norm = normalize(E);
  vec3 R_norm = normalize(-reflect(L_norm,N_norm));

  vec3 diffL = texColor0.xyz * max(dot(N_norm, L_norm), 0.0);
  diffL = clamp(diffL, 0.0, 1.0);

  vec3 specL = vec3(.8, .8, .8) * pow(max(dot(R_norm, E_norm), 0.0), shiny);
  specL = clamp(specL, 0.0, 1.0);

  Outcolor = vec4(diffL + specL + ambient*texColor0.xyz, 1);

}
