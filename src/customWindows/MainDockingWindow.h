#pragma once

#include "src/customWindows/InApplicationWindow.h"
#include "src/customWindows/InApplicationWindowImage.h"
#include "src/customWindows/SettingsWindow.h"
#include "src/SceneUtils/Scene.h"

#include <list>
#include <iostream>
#include <filesystem>



class MainDockingWindow {

public:
	MainDockingWindow();
	~MainDockingWindow();

	void renderContent();

	void newWindow(string windowName);
	void newImageWindow(string windowName, std::shared_ptr<Scene> scene);
	void newImageWindowGraph(string windowName);
	void newImageWindowGraph2(std::filesystem::path filePath, SRIP_args arg);

	

	//should probably be some event queue or something that references both.
	void pollSubwindows();
	void update();

private:
	std::list<InApplicationWindow*> subWindows;
	std::unique_ptr<SettingsWindow> settingsWindow = make_unique<SettingsWindow>(SettingsWindow());

	string generateName(string windowName, int count);
};