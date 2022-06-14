#pragma once

#include "string"
#include "imgui.h"
#include <Dependencies/glm/glm/ext/vector_float2.hpp>

using namespace std;

class InApplicationWindow {

public:
	InApplicationWindow(string windowName);

	virtual void render();
	virtual void update();

	virtual void shutdown();

	string getWindowName();

	glm::vec2 getDimensions();


private:
	string windowName;
	bool running;

	int width = 400;
	int height = 300;
};