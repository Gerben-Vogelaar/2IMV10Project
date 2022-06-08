#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class WindowCamera {

public:
	glm::mat4 getTransformMatrix();

private:
	glm::vec2 cameraOffset = glm::vec2(0.0f, 0.0f);
	float cameraZoom = 1.0f;
};