#version 330 core

uniform sampler2D texBuf;
uniform float width;
uniform float height;
in vec2 texCoord;
out vec4 color;

void main(){
//    color = vec4(1, 0, 0, 1);
    color = vec4(texture(texBuf, texCoord).rgb, 1);
}