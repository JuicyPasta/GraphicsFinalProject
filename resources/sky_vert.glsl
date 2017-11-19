#version  330 core

layout(location = 0) in vec3 vertPos;

uniform mat4 P;
uniform mat4 V;

out vec3 vTexCubeCoord;


void main() {

  gl_Position = P * V * vec4(vertPos.xyz, 1.0);
  vTexCubeCoord = vertPos.xyz;
}
