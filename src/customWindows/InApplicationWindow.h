#pragma once

#include "string"
#include "imgui.h"

using namespace std;

class InApplicationWindow {

public:
	InApplicationWindow(string windowName);

	virtual void render();
	virtual void shutdown();

	string getWindowName();

private:
	string windowName;
	bool running;

	
};