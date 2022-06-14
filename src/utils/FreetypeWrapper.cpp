//#include "FreetypeWrapper.h"
//
//FreetypeWrapper::FreetypeWrapper(Shader& shader) {
//
//    FT_Library ft;
//    // All functions return a value different than 0 whenever an error occurred
//    if (FT_Init_FreeType(&ft))
//    {
//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//        //return -1;
//    }
//
//    //TODO: fix this path!
//    //std::string font_name = "C:/Dev/2IMV10_IciclePlot/2IMV10_iciclePlot/resources/fonts/arial.ttf";
//    std::string font_name = "resources/fonts/arial.ttf";
//
//    if (font_name.empty())
//    {
//        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
//    }
//
//    //std::string font_name;
//    //std::ifstream font_name_file;
//    //font_name_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//    //try {
//    //    font_name_file.open("resources/fonts/arial.ttf");
//    //    std::stringstream font_name_stream;
//
//    //    font_qname_stream << font_name_file.rdbuf();
//    //    // close file handlers
//    //    font_name_file.close();
//    //    // convert stream into string
//    //   font_name = font_name_stream.str();
//    //}
//    //catch (std::ifstream::failure& e)
//    //{
//    //    std::cout << "ERROR::READING::FONT" << e.what() << std::endl;
//    //}
//
//    if (font_name.empty())
//    {
//        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
//        //return -1;
//    }
//
//    std::cout << font_name << std::endl;
//
//    // load font as face
//    FT_Face face;
//    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//        //return -1;
//    }
//    else {
//        // set size to load glyphs as
//        FT_Set_Pixel_Sizes(face, 0, 48);
//
//        // disable byte-alignment restriction
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//        // load first 128 characters of ASCII set
//        for (unsigned char c = 0; c < 128; c++)
//        {
//            // Load character glyph 
//            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//            {
//                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                continue;
//            }
//            // generate texture
//            unsigned int texture;
//            glGenTextures(1, &texture);
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glTexImage2D(
//                GL_TEXTURE_2D,
//                0,
//                GL_RED,
//                face->glyph->bitmap.width,
//                face->glyph->bitmap.rows,
//                0,
//                GL_RED,
//                GL_UNSIGNED_BYTE,
//                face->glyph->bitmap.buffer
//            );
//            // set texture options
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            // now store character for later use
//            Character character = {
//                texture,
//                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                static_cast<unsigned int>(face->glyph->advance.x)
//            };
//            Characters.insert(std::pair<char, Character>(c, character));
//        }
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//    // destroy FreeType once we're finished
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
//
//    /* BELOW WAS ADDED!!!*/
//    shader.use();
//    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
//}
//
//void FreetypeWrapper::RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO)
//{
//    //Enable blending, else only the quads will be printed!
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    // activate corresponding render state	
//    shader.use();
//    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
//    glActiveTexture(GL_TEXTURE0);
//    glBindVertexArray(VAO);
//
//    // iterate through all characters
//    std::string::const_iterator c;
//    for (c = text.begin(); c != text.end(); c++)
//    {
//        Character ch = Characters[*c];
//
//        float xpos = x + ch.Bearing.x * scale;
//        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//        float w = ch.Size.x * scale;
//        float h = ch.Size.y * scale;
//        // update VBO for each character
//        float vertices[6][4] = {
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos,     ypos,       0.0f, 1.0f },
//            { xpos + w, ypos,       1.0f, 1.0f },
//
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos + w, ypos,       1.0f, 1.0f },
//            { xpos + w, ypos + h,   1.0f, 0.0f }
//        };
//        // render glyph texture over quad
//        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//        // update content of VBO memory
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        // render quad
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
//    }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}