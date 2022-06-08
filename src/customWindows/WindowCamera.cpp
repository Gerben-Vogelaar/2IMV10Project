#include "WindowCamera.h"

glm::mat4 WindowCamera::getTransformMatrix()
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(cameraZoom * cameraOffset.x, cameraZoom * cameraOffset.y, 0.0f));
	transform = glm::scale(transform, glm::vec3(cameraZoom, cameraZoom, 1.0f));

	return transform;
}
