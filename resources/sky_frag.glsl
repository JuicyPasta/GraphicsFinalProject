#version 330 core

out vec4 Outcolor;
uniform samplerCube skyTexture;

in vec3 vTexCubeCoord;

void main() {
//Outcolor = vec4 (1, 0, 0, 1);
  Outcolor = texture(skyTexture, vTexCubeCoord);

}
