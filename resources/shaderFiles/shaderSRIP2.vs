#version 330 core
layout (location = 0) in vec2 Pos;   // the position variable has attribute position 0
 
uniform float colorIn;
uniform mat4 transform;
uniform mat2 rotateMatrix;

out vec3 ourColor; // output a color to the fragment shader

#define PI 3.1415926538

void main()
{
    vec2 aPos = vec2(aPos.x - 1.0f, aPos.y);

    float rotation = (Pos.x) * PI;

     mat2 rotate = 
     mat2(
        cos(rotation), sin(rotation),   //First collumn 
        -sin(rotation), cos(rotation));  //second collumn

    //vec2 bPos = rotate * vec2(aPos.x - 1.0f, aPos.y);
    vec2 bPos = rotate * vec2(aPos.x, aPos.y);
    //vec2 bPos = vec2(sin(rotation), aPos.y * cos(rotation));
    //vec2 bPos = vec2(sin(rotation), aPos.y * cos(rotation));
    gl_Position = transform * vec4(bPos, 1.0, 1.0);
    ourColor = vec3((aPos.y+1)/2, (aPos.y+1)/2, colorIn); // set ourColor to the input color we got from the vertex data
} 