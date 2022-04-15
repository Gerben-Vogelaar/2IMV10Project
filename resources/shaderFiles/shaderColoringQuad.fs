#version 330 core
out vec4 FragColor;  

uniform int totalVertex;
uniform int vertexPerQuad;

in vec3 ourColor;
in int gl_PrimitiveID;  

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    //FragColor = vec4(ourColor, 1.0);
    float i = gl_PrimitiveID - mod(float(gl_PrimitiveID), vertexPerQuad);
    float j = float(totalVertex)/(4 * vertexPerQuad);
    float k = i / j;

    //FragColor = vec4(k, 0.5f, 0.5f, 1.0f);
    FragColor = vec4(rand(vec2(i,j)), 0.5f, 0.5f, 1.0f);

}

