#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include<sstream>
//#include "newick/newickParser.h"

#include <math.h>

#include "shader/shader.h"

#include "newick/newickTree.h"
#include "iciclePlot/IciclePlot.h"
#include "iciclePlot/SpaceReclaimingIciclePlot.h"


#include "src/iciclePlot/TestingBezierCurverImpl.h"

void processInput(GLFWwindow* window, float deltaTime);
void window_size_callback(GLFWwindow* window, int width, int height);
void draw(unsigned int VAO, int sizeTest);
void draw2(unsigned int VAO, SpaceReclaimingIciclePlot plot);

void processNewPlot(GLFWwindow* window, SpaceReclaimingIciclePlot& plot, float hValue, Newick tree, SRIP1_arg& args);

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
    ifile.open("./resources/newickTrees/kmer_distance.newick.txt");
    //ifile.open("./resources/newickTrees/life.txt");
    //ifile.open("./resources/newickTrees/ani.newick.txt");
    stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());    
    Newick newick = Newick(as);

    float valueW = 1.0f;
    float hValue = 0.2f;
    /*glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "hValue"), hValue);
    */

    //SRIP1_arg args1;
    //args1.setGamma(0.00f);
    //args1.seth(0.04f);
    //args1.setRho(0.0f);
    //args1.setW(2.0f); 
    
    SRIP1_arg args1;
    args1.setGamma(0.02f);
    args1.seth(0.04f);
    args1.setRho(0.6f);
    args1.setW(2.0f);

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

    SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args2, false, 50);

    cout << glfwGetTime() << endl;

    //IciclePlot plot = IciclePlot(newick);
    //SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(valueW, hValue, 0.0f, 0.0f, newick);

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

    /* load shaders*/

    Shader ourShader("resources/shaderFiles/shaderSRIP2.vs", "resources/shaderFiles/shaderSRIP2.fs"); // you can name your shader files however you like
    Shader ourShader2("resources/shaderFiles/shaderTest.vs", "resources/shaderFiles/shaderTest.fs"); // you can name your shader files however you like

    //float vertices[] = {
    //    // positions         
    //     0.5f, -0.5f, 0.0f,   // bottom right
    //    -0.5f, -0.5f, 0.0f,   // bottom left
    //     0.5f,  0.5f, 0.0f,   // top 
    //     0.5f, 0.5f, 0.0f,   // bottom right
    //    -0.5f, -0.5f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,   // top 
    //};

    TestingBezierCurverImpl test = TestingBezierCurverImpl();
    test.generateElements(100, 0.0f, 0.0f, 2.0f, 0.4f);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //DELETE: for triangle drawing only
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, test.size * sizeof(float), test.elements, GL_STATIC_DRAW);
    
    //TO DRAW OUR PLOT!
    glBufferData(GL_ARRAY_BUFFER, plot.getVertexDataArraySize() * sizeof(float), plot.getVertexDataArray(), GL_STATIC_DRAW);

    //DRAWING the plot: number of elements * sizeof(float) and just the pointer to vertices

    //DELETE: for triangle drawing only
        // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //---------------

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


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

        /* Render here */
        //draw(VAO, test.size);

        draw2(VAO, plot);

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

        //cout << (hValue - 0.5f) * 2 << endl;

        ourShader.setMat4("transform", transform);
        ourShader.setMat2("rotateMatrix", rotate);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("My name is window, ImGUI window");
        ImGui::Text("Hello there advanturer!");
        ImGui::SliderFloat("hValue", &hValue, 0.0f, valueW);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//        draw axis with ourShader2
        //ourShader2.use();

        //glBegin(GL_LINES);
        //glVertex2f(0, 1);
        //glVertex2f(0, -1);
        //glVertex2f(1, 0);
        //glVertex2f(-1, 0);
        //glEnd();

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

void draw(unsigned int VAO, int sizeTest) {
    glClear(GL_COLOR_BUFFER_BIT);

    //BACKGROUND
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO); 
    glDrawArrays(GL_TRIANGLES, 0, sizeTest); 

}

/* Contains the "old" code for reference*/
void draw2(unsigned int VAO, SpaceReclaimingIciclePlot plot) {
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
        //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, plot.getVertexDataArraySize()); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!

    //glBegin(GL_LINES);
    //glVertex2f(0, 1);
    //glVertex2f(0, -1);
    //glVertex2f(1, 0);
    //glVertex2f(-1,0);
    //glEnd();
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
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !Pressed_KEY_MINUS) {
        Pressed_KEY_MINUS = true;
        zoom -= 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && !Pressed_KEY_EQUAL) {
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
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE && Pressed_KEY_MINUS) {
        Pressed_KEY_MINUS = false;
    }else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE && Pressed_KEY_EQUAL) {
        Pressed_KEY_EQUAL = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && Pressed_KEY_1) {
        Pressed_KEY_1 = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && Pressed_KEY_2) {
        Pressed_KEY_2= false;
    }
}

void processNewPlot(GLFWwindow* window, SpaceReclaimingIciclePlot& plot, float hValue, Newick tree, SRIP1_arg& args) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {

        cout << "pressed 1" << endl;

        args.gamma = hValue;

       // plot = (SpaceReclaimingIciclePlot(tree, args));
    }
}