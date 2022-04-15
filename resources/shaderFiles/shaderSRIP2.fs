#version 330 core
out vec4 FragColor;  

uniform int totalVertex;

in vec3 ourColor;
in int gl_PrimitiveID ;  

void main()
{
    //FragColor = vec4(ourColor, 1.0);

    float i = float(gl_PrimitiveID);
    float j = float(totalVertex)/3;
    float k = i / j;

    if(mod(gl_PrimitiveID, 2.0)==0.0){        
        FragColor = vec4(k, 0.5f, 0.5f, 1.0f);
    }
    else {
        FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }
        
}