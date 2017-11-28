#version  330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec2 vTexCoord;
out vec3 L;
out vec3 E;
out vec3 N;

void main() {
  vec3 source = vec3(1, 1, 1);

  gl_Position = P * V * M * vec4(vertPos.xyz, 1.0);
  vec4 worldCord = M * vec4(vertPos, 1);

  L = normalize(source - worldCord.xyz);
  N = (M * vec4(vertNor, 0.0)).xyz;
  E = -worldCord.xyz;

  vTexCoord = vertTex;
}
