#include <Dependencies/glm/glm/ext/vector_float3.hpp>
#include <Dependencies/glm/glm/ext/matrix_float4x4.hpp>
#include <Dependencies/glm/glm/ext/matrix_transform.hpp>

#include <iostream>

#include <glm/gtx/string_cast.hpp>


class SceneCamera {

public:
	SceneCamera(const float minZoom = 0.01f, const float maxZoom = 10.0f, const float ZOOM_PER_SCROLL_UNIT = 0.1f);

	glm::mat4 getTransformationMatrix();
	void incrementZoomLegalValue(const glm::vec3 zoom);
	void incrementZoomLegalValue(float scrollUnits);

	void addOffset(glm::vec3 offset);

	void printZoom();

private:
	glm::vec3 zoom;
	glm::vec3 offsetCenter;

	const float MIN_ZOOM;
	const float MAX_ZOOM;
	const float ZOOM_PER_SCROLL_UNIT;
};