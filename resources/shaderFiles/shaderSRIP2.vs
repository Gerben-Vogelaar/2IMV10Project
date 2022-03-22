#version 410 core
layout (location = 0) in vec2 Pos;   // the position variable has attribute position 0
 
uniform float colorIn;
uniform mat4 transform;
uniform mat2 rotateMatrix;

uniform int rotatePlot;

out vec3 ourColor; // output a color to the fragment shader

#define PI 3.1415926538

void main()
{
    //angle to translate
    float r = -(Pos.x - 1.0f)  * PI; //1.05f

	mat2 rotate = 
     mat2(
       cos(r), sin(r),   //First collumn 
       -sin(r), cos(r));  //second collumn

    vec2 rPos = rotate * vec2(0, Pos.y);
    //gl_Position = transform * vec4(rPos, 1.0, 1.0);
    
    if (rotatePlot == 0){
        gl_Position = transform * vec4(rPos, 1.0, 1.0);
    } else {
      gl_Position = transform * vec4(Pos, 1.0, 1.0);
    } 
    ourColor = vec3(1.0f, 0.0f, colorIn); // set ourColor to the input color we got from the vertex data
} 