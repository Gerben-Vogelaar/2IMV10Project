#pragma once

#include "src/shader/Shader.h"
#include "GL/glew.h"

class Scene {

public:

	Scene(Shader* shader, int sizeData, float* data, int totalVertex, int screenWidth, int screenHeight, int GLint=3);

	virtual void updateScene();

	//void setData(float* data, int sizeData, GLenum mode);

	unsigned int getTextureBuffer();

	void setDrawConfig(int totalVertex);

private:
	Shader* shader;

	float* data;
	int sizeData;

	unsigned int textureBuffer;
	unsigned int frameBuffer;
	unsigned int VAO, VBO;

	//required for the draw operations
	int totalVertex;
	int sizeAttributes; //elements per vertex e.g. vec3 = 3 etc. (assuming no additional layout elements)
};