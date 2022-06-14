#pragma once

#include "src/customWindows/InApplicationWindow.h"
#include "src/customWindows/InApplicationWindowImage.h"

#include <list>
#include <iostream>
#include <src/SceneUtils/Scene.h>


class MainDockingWindow {

public:
	~MainDockingWindow();

	void renderContent();

	void newWindow(string windowName);
	void newImageWindow(string windowName, std::shared_ptr<Scene> scene);
	void renderSubwindows();

private:
	std::list<InApplicationWindow*> subWindows;


};