#version 330 core

layout(location = 0) in vec3 vertPos;
uniform mat4 depthMVP;

void main(){
    gl_Position =  depthMVP * vec4(vertPos,1);
}