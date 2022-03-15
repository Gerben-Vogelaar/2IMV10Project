#version 330 core
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
 
uniform float colorIn;
uniform mat4 transform;

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = transform * vec4(aPos, 1.0, 1.0);
    ourColor = vec3((aPos.x+1)/2, (aPos.y+1)/2, colorIn); // set ourColor to the input color we got from the vertex data
} 