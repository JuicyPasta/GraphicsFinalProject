#version  330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;
uniform mat4 depthBiasMVP;

out vec2 vvTexCoord;
out vec3 vL;
out vec3 vE;
out vec3 vN;
out vec3 vertRaw;
out vec4 vShadowCoord;


void main() {
  vec3 source = vec3(1, 1, 1);

  gl_Position = M * vec4(vertPos.xyz, 1.0);

// MIGHT NEED TO ADD M
  vShadowCoord = depthBiasMVP * M * vec4(vertPos,1);

  vec4 worldCord = M * vec4(vertPos, 1);

  vL = normalize(source - worldCord.xyz);
  vN = (M * vec4(vertNor, 0.0)).xyz;
  vE = -worldCord.xyz;

  vvTexCoord = vertTex;
  vertRaw = vertPos.xyz;
}
