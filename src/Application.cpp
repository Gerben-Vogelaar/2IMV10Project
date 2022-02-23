#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include<sstream>
//#include "newick/newickParser.h"

#include "newick/newickTree.h"
#include "iciclePlot/IciclePlot.h"
#include "iciclePlot/SpaceReclaimingIciclePlot.h"

int main(void)
{
    std::cout << "Hello" << std::endl;

    ifstream ifile;
    //ifile.open("./resources/newickTrees/test.txt");
    ifile.open("./resources/newickTrees/test.txt");
    stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());    
    Newick newick = Newick(as);

    
    //IciclePlot plot = IciclePlot(newick);
    SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(1.0f, 0.2f, 0.0f, 0.0f, newick);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        cout << "Failed initializing Glew" << endl;
    }

    cout << "OpenGL version:  " << glGetString(GL_VERSION) << endl;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //DRAWING the plot: number of elements * sizeof(float) and just the pointer to vertices
    //glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), vertices2, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //-------------------------
    ////Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    //unsigned int VAO, VBO, EBO;

    //glGenVertexArrays(1, &VAO);

    //glGenBuffers(1, &EBO);
    //glGenBuffers(1, &VBO);

    ////PROBABLY WRONG SINCE WE NEED indices dereferenced.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ////bind the vertex array object first, then bind and set vertex buffer(s), and then configure bertex attribute(s).
    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof())

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(0.5f, -0.5f);
        glEnd();*/
        

        //BACKGROUND
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized - 84
        glDrawArrays(GL_TRIANGLES, 0, plot.getVertexDataArraySize()); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}