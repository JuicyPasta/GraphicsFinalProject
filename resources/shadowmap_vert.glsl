#version 330 core

layout(location = 0) in vec3 vertPos;
uniform mat4 depthMVP;
uniform mat4 M;

void main(){
    gl_Position =  depthMVP * M * vec4(vertPos,1);
}