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
#include <glm/gtx/string_cast.hpp>

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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void highlightSelectedNodes(bool& search, bool rotatePlot, SpaceReclaimingIciclePlot& plot);

// FOR TESTING RUNTIME ONLY:
void testSpeedAlgorithms(string path);

int screen_width = 800;
int screen_height = 600;

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

//contains x and y pos of the mouse after a click event
double xpos_mouse, ypos_mouse;
bool selectingNode = false;
TreeNode* selectedNode = NULL;
float data_selected[8  * QUAD_PRECISION];
//float data_selected[8];
string selectedNodeText;

int main(void)
{
    std::cout << "aswd -> move the screen coordinates \n - and + -> to zoom in and out" << endl;

    //testSpeedAlgorithms("./resources/newickTrees/ani.newick.txt");
    //testSpeedAlgorithms("./resources/newickTrees/core_snp_tree.newick.txt");
    //testSpeedAlgorithms("./resources/newickTrees/gene_distance.newick.txt");
    //testSpeedAlgorithms("./resources/newickTrees/kmer_distance.newick.txt");
    //testSpeedAlgorithms("./resources/newickTrees/life.txt");
    //testSpeedAlgorithms("./resources/newickTrees/mlsa.newick.txt");
    //testSpeedAlgorithms("./resources/newickTrees/pepper_001.txt");

    ifstream ifile;
    //ifile.open("./resources/newickTrees/life.txt");
    ifile.open("./resources/newickTrees/life.txt");
    
    stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());    
    Newick newick = Newick(as);
    newick.printStatistics();

    float valueW = 1.0f;
    float hValue = 0.2f;

    SRIP1_arg args1;
    args1.setGamma(0.025f);
    args1.seth(0.25f);
    args1.setRho(0.8f);
    args1.setW(2.0f); 

    SRIP2_arg args2;
    args2.setGamma(0.1f);
    args2.seth(0.085f);
    args2.setRho(0.1f);
    args2.setW(2.0f);
    args2.setEpsilon(2.0f);
    args2.setSigma(1.0f);
    args2.setLambda(30);


    //SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, true, QUAD_PRECISION);

    SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, false, QUAD_PRECISION);


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screen_width, screen_height, "Hello World", NULL, NULL);
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

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    imGUIWrapper imGuiWrapper = imGUIWrapper(window);

    /* load shaders*/

    Shader ourShader("resources/shaderFiles/shaderSRIP2.vs", "resources/shaderFiles/shaderColoringQuad.fs");
    Shader highlightShader("resources/shaderFiles/shaderSRIP2.vs", "resources/shaderFiles/shaderTest.fs");
    Shader shaderText("resources/shaderFiles/textShader.vs", "resources/shaderFiles/textShader.fs");
    
    FreetypeWrapper ft = FreetypeWrapper(shaderText);

    unsigned int VBO, VAO, VBO_text, VAO_text, VBO_highlight, VAO_highlight;

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

    glGenVertexArrays(1, &VAO_highlight);
    glGenBuffers(1, &VBO_highlight);
    glBindVertexArray(VAO_highlight);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_highlight);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

        ourShader.setInt("rotatePlot", rotatePlot?0:1);
        ourShader.setInt("totalVertex", plot.getVertexDataArraySize());
        ourShader.setInt("vertexPerQuad", QUAD_PRECISION);

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
                plot = SpaceReclaimingIciclePlot(newick, args, false, QUAD_PRECISION);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);
                
                //draw2(VAO, &plot);
            } else if (as == ALGORITHM_1_E) {
                SRIP1_arg args = imGuiWrapper.getArgs1();
                //plot = SpaceReclaimingIciclePlot(newick, args1, false, QUAD_PRECISION);
                plot = SpaceReclaimingIciclePlot(newick, args, true, QUAD_PRECISION);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);
            }
            else if (as == ALGORITHM_2) {
                SRIP2_arg args = imGuiWrapper.getArgs2();
                plot = SpaceReclaimingIciclePlot(newick, args, false, QUAD_PRECISION);

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

        highlightSelectedNodes(selectingNode, rotatePlot, plot);

        ft.RenderText(shaderText, selectedNodeText, 100.0f, 10.0f, 0.5f, glm::vec3(0.0f, 0.1f, 0.9f), VAO_text, VBO_text);

        if (selectedNode != NULL) {
            highlightShader.use();
            highlightShader.setInt("rotatePlot", rotatePlot ? 0 : 1);

            highlightShader.setMat4("transform", transform);
            highlightShader.setMat2("rotateMatrix", rotate);

            glBindVertexArray(VAO_highlight);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_highlight);
            glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * QUAD_PRECISION, &data_selected, GL_STATIC_DRAW);

            glDrawArrays(GL_QUADS, 0, 8*QUAD_PRECISION);
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAO_highlight);
    glDeleteBuffers(1, &VBO_highlight);

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

    glBindVertexArray(VAO); 
    glDrawArrays(GL_QUADS, 0, iciclePlot->getVertexDataArraySize()); 
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

    screen_width = width;
    screen_height = height;
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
        if (zoom <= 0.1f) {
            return;
        }
        zoom -= 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { // && !Pressed_KEY_EQUAL) {
        Pressed_KEY_EQUAL = true;
        zoom += 0.1f;
    }
    //else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !Pressed_KEY_1) {
    //    Pressed_KEY_1 = true;
    //    rotatePlot = !rotatePlot;
    //}
    //else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !Pressed_KEY_2) {
    //    Pressed_KEY_2 = true;
    //    rasterize = !rasterize;

    //    if (rasterize) {
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    }
    //    else {
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    }
    //}
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos_mouse, &ypos_mouse);       

        selectingNode = true;
    }
}

void highlightSelectedNodes(bool& search, bool rotatePlot, SpaceReclaimingIciclePlot& plot) {
    //no new node was selected
    if (!search) {
        return;
    }

    else {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(side, up, 0.0f));
        transform = glm::scale(transform, glm::vec3(1/zoom, 1/zoom, 1.0f));

        //map 
        float xpos_t = ((xpos_mouse - (screen_width / 2.0f))  / screen_width * 2.0f);
        float ypos_t = -(ypos_mouse - (screen_height/2.0f)) / screen_height * 2.0f;

        glm::vec4 trans = transform * glm::vec4(xpos_t, ypos_t, -1.0f, -1.0f);

        //cout << "normal point: (" << xpos_t << "," << ypos_t << ") -> (" << trans.x << " - 1.0f," << trans.y << ") ";
        //cout << "zoom: (" << zoom << ") up, side( " << up << "," << side << ")";

        float x_trans = trans.x;
        float y_trans = trans.y;

        if (rotatePlot) {

            x_trans += 1.0f;

            float y = std::sqrt(std::pow(trans.x, 2) + std::pow(trans.y, 2));

            //cos-1 [ (a · b) / (|a| |b|) ]
            float r = glm::dot(glm::vec2(0.0f, y), glm::vec2(trans.x, trans.y)) / (glm::length(glm::vec2(0.0f, y)) * glm::length(glm::vec2(trans.x, trans.y)));

            float x = - std::acos(r) / 3.1415926538;

            //cout << "x,y rotate " << x << " " << y <<  " r: " << r << endl;

            if (trans.x >= 0.0f) {
                x = -x;
            }

            x_trans = x + 1.0f; //we x_trans to points in -1, 1; but indexing requires these points to be in [0,2];
            y_trans = y;

           /* cout << "-> (" << x_trans << " " << y_trans << ") | zoom: (" << zoom << ") up, side(" << up << ", " << side << ")";*/
        }
        
        cout << endl;

        // we do not trans.x - 1.0f since values in index range from 0-2 not -1 - 1.
        TreeNode* node = plot.selectTreeNode(x_trans, y_trans);

        if (node != NULL) {
            cout << "selected node succesfully:     ";
            TreeNode n = (*node);
            n.print();
            selectingNode = true;
            selectedNode = node;

            int index = 0;

            plot.drawQuadrangleByQuadrangleHorizontalRef(data_selected, index, node->point1, node->point2, node->point3, node->point4, QUAD_PRECISION);

            selectedNodeText = "selectedNode: " + n.root_label;

        } //NODE equals zero
        else {
            selectedNode = NULL;
        }
    }

    selectingNode = false;
}

void testSpeedAlgorithms(string path) {
    ifstream ifile;
    ifile.open(path);

    //ifile.open("./resources/newickTrees/pepper_001.txt");

    stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());
    Newick newick = Newick(as);

    newick.printStatistics();

    float valueW = 1.0f;
    float hValue = 0.2f;

    SRIP1_arg args1;
    args1.setGamma(0.1f);
    args1.seth(0.1f);
    args1.setRho(0.1f);
    args1.setW(2.0f);

    SRIP2_arg args2;
    args2.setGamma(0.1f);
    args2.seth(0.1f);
    args2.setRho(0.1f);
    args2.setW(2.0f);
    args2.setEpsilon(2.0f);
    args2.setSigma(1.0f);
    args2.setLambda(30);

    cout << "Algorithm 1,1-E, 2 respectivelly, file" << path << endl;

    //Yes I do want that, it is just for testing bruv
    SpaceReclaimingIciclePlot(newick, args1, false, QUAD_PRECISION);
    SpaceReclaimingIciclePlot(newick, args1, true, QUAD_PRECISION);
    SpaceReclaimingIciclePlot(newick, args2, false, QUAD_PRECISION);
}

//Gets mouse position on pressing
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        cout << "MOUSE PRESSED" << endl;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        cout << xpos << " " << ypos << endl;
    }
}