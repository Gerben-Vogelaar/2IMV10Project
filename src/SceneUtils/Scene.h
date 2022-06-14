#pragma once

#include "src/shader/Shader.h"
#include "GL/glew.h"
#include "glm/glm.hpp"

#include <src/SceneUtils/SceneCamera.h>

class Scene {

public:

	Scene(std::shared_ptr<Shader>& shader, int sizeData, float* data, int totalVertex, int screenWidth, int screenHeight, int GLint=3, bool ZOOM_ALLOWED = false);

	virtual void updateScene();

	//void setData(float* data, int sizeData, GLenum mode);

	unsigned int getTextureBuffer();

	void setDrawConfig(int totalVertex);
	void setDrawMode(GLenum mode);

	void zoomScene(float scrollUnits);
	void moveScene(glm::vec2 offset);

	void setViewport(int x, int y, int width, int height);


	void cameraDebug();

private:
	bool ZOOM_ALLOWED;

	std::shared_ptr<Shader> shader;

	float* data;
	int sizeData;

	SceneCamera camera = SceneCamera(); // = std::make_unique<SceneCamera>(new SceneCamera());

	unsigned int textureBuffer;
	unsigned int frameBuffer;
	unsigned int VAO, VBO;

	//required for the draw operations
	int totalVertex;
	int sizeAttributes; //elements per vertex e.g. vec3 = 3 etc. (assuming no additional layout elements)
	unsigned int drawMode = GL_TRIANGLES;
};