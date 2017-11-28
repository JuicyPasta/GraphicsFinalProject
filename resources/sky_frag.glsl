#version 330 core

out vec4 Outcolor;
uniform samplerCube skyTexture;

in vec3 vTexCubeCoord;

void main() {
//Outcolor = vec4 (1, 0, 0, 1);

  Outcolor = texture(skyTexture, vTexCubeCoord);
//
//  float blueTint = sin(vTexCubeCoord.x) + cos(vTexCubeCoord.y) + sin(vTexCubeCoord.x+vTexCubeCoord.y);


//  Outcolor = vec4(texColor.r, texColor.g, texColor.b + blueTint, 1);


}
