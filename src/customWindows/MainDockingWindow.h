#pragma once

#include "src/customWindows/InApplicationWindow.h"
#include "src/customWindows/InApplicationWindowImage.h"

#include <list>
#include <iostream>


class MainDockingWindow {

public:
	~MainDockingWindow();

	void renderContent();

	void newWindow(string windowName);
	void newImageWindow(string windowName, unsigned int textureBuffer);
	void renderSubwindows();

private:
	std::list<InApplicationWindow*> subWindows;


};