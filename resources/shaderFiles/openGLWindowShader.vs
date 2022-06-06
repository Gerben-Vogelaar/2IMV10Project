#version 330 core

in vec3 colorIn;
out vec3 color;

void main(){
    //color = texture( renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz;
    color = colorIn;
}