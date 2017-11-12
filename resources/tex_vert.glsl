#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 eye;
out vec3 fragNor;
out vec3 WPos;
out vec2 texCoord;
out vec3  viewDir;


void main() {
	gl_Position = P * V * M * vertPos;
	fragNor = (V * M * vec4(vertNor, 0.0)).xyz;
	WPos = vec3(V*M*vertPos);
	texCoord = (vertPos.xy+vec2(1, 1))/2.0;
	viewDir = normalize(eye - vec3(V * M * vertPos));
}
