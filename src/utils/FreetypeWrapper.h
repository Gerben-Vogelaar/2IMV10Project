//#pragma once
//
//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//#include <glm/glm.hpp>
//#include <map>
//#include <filesystem>
//
//#include "src/shader/shader.h";
//
//struct Character {
//    unsigned int TextureID; // ID handle of the glyph texture
//    glm::ivec2   Size;      // Size of glyph
//    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
//    unsigned int Advance;   // Horizontal offset to advance to next glyph
//};
//
//class FreetypeWrapper {
//
//public: 
//
//	FreetypeWrapper(Shader& shader);
//    //FreetypeWrapper(glm::mat4 projectionMatrix, Shader& shader);
//    std::map<GLchar, Character> Characters;
//
//    
//
//    void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO);
//
//private:
//    //Shader shader;
//    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(600), 0.0f, static_cast<float>(800)); //TODO: make it dynamic!!!!
//};