//#ifndef SHADER_H
//#define SHADER_H

/*FROM: learnOpenGL.com */

#pragma once

//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);
        // activate the shader
        // ------------------------------------------------------------------------
    void use();
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const;
    //-------------------------------------------------------------------- -
        void setInt(const std::string & name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const;

    void setMat4(const std::string& name, glm::mat4 model);
    
    void setVec3(const std::string& name, glm::vec3 value);

    void setMat2(const std::string& name, glm::mat2 model);
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};

//#endif