#pragma once

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/utils/FreetypeWrapper.h"

#include <fstream>
#include<sstream>

#include <math.h>

#include "shader/shader.h"

#include "newick/newickTree.h"
#include "iciclePlot/IciclePlot.h"
#include "iciclePlot/SpaceReclaimingIciclePlot.h"


#include "src/utils/imGUIWrapper.h"

void processInput(GLFWwindow* window, float deltaTime);
void window_size_callback(GLFWwindow* window, int width, int height);
void draw(unsigned int VAO, int sizeTest);
void draw2(unsigned int VAO, SpaceReclaimingIciclePlot* plot);

const int QUAD_PRECISION = 50;

const float CAMERA_SPEED = 0.75f;

bool rotatePlot = false;
bool rasterize = false;

float zoom = 1.0f;
float up = 0.0f;
float side = 0.0f;

bool Pressed_KEY_1 = false;
bool Pressed_KEY_2 = false;
bool Pressed_KEY_MINUS = false;
bool Pressed_KEY_EQUAL = false;

int main(void)
{
    ifstream ifile;
    //ifile.open("./resources/newickTrees/kmer_distance.newick.txt");
    ifile.open("./resources/newickTrees/life.txt");
    //ifile.open("./resources/newickTrees/test2.txt");
    //ifile.open("./resources/newickTrees/ani.newick.txt");
    stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());    
    Newick newick = Newick(as);

    float valueW = 1.0f;
    float hValue = 0.2f;

    SRIP1_arg args1;
    args1.setGamma(0.00f);
    args1.seth(0.04f);
    args1.setRho(0.3f);
    args1.setW(1.0f); 
    
    //SRIP1_arg args1;
    //args1.setGamma(0.02f);
    //args1.seth(0.08f);
    //args1.setRho(0.0f);
    //args1.setW(2.0f);

    SRIP2_arg args2;
    args2.setGamma(0.1f);
    args2.seth(0.085f);
    args2.setRho(0.4f);
    args2.setW(2.0f);
    args2.setEpsilon(2.0f);
    args2.setSigma(1.0f);
    args2.setLambda(30);

    

    //IciclePlot SRIP1
    //SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, false, 50);

    cout << glfwGetTime() << endl;

    //SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args2, false, 50);
    //SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, true, QUAD_PRECISION);
    SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, true);

    cout << glfwGetTime() << endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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

    imGUIWrapper imGuiWrapper = imGUIWrapper(window);

    /* load shaders*/

    //Shader ourShader("resources/shaderFiles/shaderSRIP2.vs", "resources/shaderFiles/shaderSRIP2.fs"); // you can name your shader files however you like
    //Shader ourShader2("resources/shaderFiles/shaderTest.vs", "resources/shaderFiles/shaderTest.fs"); // you can 
    Shader ourShader("resources/shaderFiles/shaderSRIP2.vs", "resources/shaderFiles/shaderColoringQuad.fs");
    Shader shaderText("resources/shaderFiles/textShader.vs", "resources/shaderFiles/textShader.fs");

    FreetypeWrapper ft = FreetypeWrapper(shaderText);

    unsigned int VBO, VAO, VBO_text, VAO_text;

    //Graph buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO_text);
    glGenBuffers(1, &VBO_text);
    glBindVertexArray(VAO_text);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    float time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float deltaTime = time - glfwGetTime();
        time = glfwGetTime();

        processInput(window, deltaTime);

        //processNewPlot(window, plot, hValue, newick, args1);
        glfwSetWindowSizeCallback(window, window_size_callback);

        //ourShader.setFloat("colorIn", 0.0f);
        ourShader.use();

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(zoom * side, zoom * up, 0.0f));
        transform = glm::scale(transform, glm::vec3(zoom, zoom, 1.0f));
        //transform = glm::rotate(transform, 3.14f, glm::vec3(0.0f, 0.0f, 1.0f));

        float rotation = 3.14f;

        glm::mat2 rotate = glm::mat2(1.0f);
        /*rotate[0][0] = cos(rotation);
        rotate[0][1] = -sin(rotation);
        rotate[1][0] = sin(rotation);
        rotate[1][1] = cos(rotation);*/

        ourShader.setInt("rotatePlot", rotatePlot?1:0);
        ourShader.setInt("totalVertex", plot.getVertexDataArraySize());
        ourShader.setInt("vertexPerQuad", QUAD_PRECISION);

        //cout << (hValue - 0.5f) * 2 << endl;

        ourShader.setMat4("transform", transform);
        ourShader.setMat2("rotateMatrix", rotate);

        draw2(VAO, &plot);

        imGuiWrapper.renderInterface();

        rotatePlot = imGuiWrapper.getRotate();
        rasterize = imGuiWrapper.getRasterize(); //FIX since it collides with the key input methods!

        if (rasterize) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
            
        if (imGuiWrapper.pollNewPlot()) {
            AlgorithmSelected as = imGuiWrapper.getAlgorithmSelected();

            if (as == ALGORITHM_1) {
                cout << "algorithm 1 selected" << endl;

                SRIP1_arg args = imGuiWrapper.getArgs1();
                //plot = SpaceReclaimingIciclePlot(newick, args1, false, QUAD_PRECISION);
                plot = SpaceReclaimingIciclePlot(newick, args, true);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);
                
                //draw2(VAO, &plot);
            }
            else if (as == ALGORITHM_2) {
                SRIP2_arg args = imGuiWrapper.getArgs2();
                //plot = SpaceReclaimingIciclePlot(newick, args2, false, QUAD_PRECISION);
                plot = SpaceReclaimingIciclePlot(newick, args, true);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);
                //draw2(VAO, plot);
            }
            else {
                //SHOULD NEVER HAPPEN!!!
                cout << "no algorihtm selected, dafuq" << endl;
            }
        };

        ft.RenderText(shaderText, "draw the text here", 100.0f, 100.0f, 0.5f, glm::vec3(1.0f, 0.1f, 0.1f), VAO_text, VBO_text);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

/* Contains the "old" code for reference*/
void draw2(unsigned int VAO, SpaceReclaimingIciclePlot* iciclePlot) {
    glClear(GL_COLOR_BUFFER_BIT);

    //BACKGROUND
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized - 84
    glDrawArrays(GL_QUADS, 0, iciclePlot->getVertexDataArraySize()); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
}

void draw(unsigned int VAO, int sizeTest) {
    glClear(GL_COLOR_BUFFER_BIT);

    //BACKGROUND
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO); 
    glDrawArrays(GL_TRIANGLES, 0, sizeTest); 

}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        up += deltaTime * CAMERA_SPEED;
    }else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        up -= deltaTime* CAMERA_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        side -= deltaTime * CAMERA_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        side += deltaTime * CAMERA_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { // && !Pressed_KEY_MINUS) {
        Pressed_KEY_MINUS = true;
        zoom -= 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { // && !Pressed_KEY_EQUAL) {
        Pressed_KEY_EQUAL = true;
        zoom += 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !Pressed_KEY_1) {
        Pressed_KEY_1 = true;
        rotatePlot = !rotatePlot;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !Pressed_KEY_2) {
        Pressed_KEY_2 = true;
        rasterize = !rasterize;

        if (rasterize) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    /*else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE && Pressed_KEY_MINUS) {
        Pressed_KEY_MINUS = false;
    }else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE && Pressed_KEY_EQUAL) {
        Pressed_KEY_EQUAL = false;
    }*/
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && Pressed_KEY_1) {
        Pressed_KEY_1 = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && Pressed_KEY_2) {
        Pressed_KEY_2= false;
    }
}
