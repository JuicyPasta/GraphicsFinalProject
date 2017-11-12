#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 L;
out vec3 H;
out vec3 fragNor;

void main() {
    vec3 source = vec3(0, 2, 2);
	gl_Position = P * V * M * vertPos;
	fragNor = (M * vec4(vertNor, 0.0)).xyz;
	vec4 worldCord = M * vertPos;

    L = normalize(source - worldCord.xyz);
    H = normalize(L + (-worldCord.xyz));
}
