#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 L;
out vec3 E;
out vec3 N;

void main() {
    vec3 source = vec3(0, 2, 2);

	gl_Position = P * V * M * vertPos;
	vec4 worldCord = M * vertPos;

	N = (M * vec4(vertNor, 0)).xyz;
    E = -worldCord.xyz;
    L = normalize(source - worldCord.xyz);
}
