#version 330 core
uniform sampler2D diffuseTexture;

out vec4 Outcolor;

in vec2 vTexCoord;

void main() {
  vec4 texColor0 = texture(diffuseTexture, vTexCoord);

  Outcolor = vec4(texColor0.r, texColor0.r, texColor0.r, 1);
//Outcolor = texColor0;

//Outcolor = vec4(1, 1, 1, 1);
}
