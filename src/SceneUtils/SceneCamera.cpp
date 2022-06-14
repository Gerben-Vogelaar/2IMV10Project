#include "SceneCamera.h"


SceneCamera::SceneCamera(const float minZoom, const float maxZoom, const float ZOOM_PER_SCROLL_UNIT) : MIN_ZOOM(minZoom), MAX_ZOOM(maxZoom), ZOOM_PER_SCROLL_UNIT(ZOOM_PER_SCROLL_UNIT), zoom(glm::vec3(1.0f)), offsetCenter(glm::vec3(0.0f, 0.0f, 0.0f)){}

glm::mat4 SceneCamera::getTransformationMatrix()
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, offsetCenter);
	//transform = glm::translate(transform, glm::vec3(-0.1f, 0.2f, 0.0f));
	transform = glm::scale(transform, zoom);
	//transform = glm::scale(transform, glm::vec3(2.0f, 1.5f, 3.0f));

	return transform;
}

void SceneCamera::incrementZoomLegalValue(float scrollUnits) {
	incrementZoomLegalValue(glm::vec3(scrollUnits * ZOOM_PER_SCROLL_UNIT, scrollUnits * ZOOM_PER_SCROLL_UNIT, 1.0f));
}

void SceneCamera::addOffset(glm::vec3 offset)
{
	this->offsetCenter += glm::vec3(-offset.x, offset.y, offset.z);
}

void SceneCamera::incrementZoomLegalValue(glm::vec3 zoomIncr)
{
	zoom += glm::vec3(zoomIncr.x, zoomIncr.y, 0.0f);

	if (zoom.x <= MIN_ZOOM) {
		zoom.x = MIN_ZOOM;
	}
	else if (zoom.x > MAX_ZOOM) {
		zoom.x = MAX_ZOOM;
	}
	
	
	if (zoom.y <= MIN_ZOOM) {
		zoom.y = MIN_ZOOM;
	}
	else if (zoom.y > MAX_ZOOM) {
		zoom.y = MAX_ZOOM;
	}
}

void SceneCamera::printZoom()
{
	std::cout << "Zoom  :: " << glm::to_string(zoom) << std::endl;
	std::cout << "Offset:: " << glm::to_string(offsetCenter) << std::endl;

	std::cout << glm::to_string(getTransformationMatrix()) << std::endl;
}

