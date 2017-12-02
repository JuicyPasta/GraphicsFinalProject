#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec2 vertTex;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec2 texCoord;

void main() {
	gl_Position = P * V * M * vertPos;
	texCoord = (vertTex+vec2(1, 1)) / 4.0;
}