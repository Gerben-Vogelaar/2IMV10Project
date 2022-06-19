#pragma once

#include "InApplicationWindow.h"
#include <src/iciclePlot/SpaceReclaimingIciclePlot.h>
#include <src/SceneUtils/Scene.h>

#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

//TODO: remove the SRIP1_args and SRIP2_args eventually since they contain overalapping data

class SettingsWindow : public InApplicationWindow {

public: 
	SettingsWindow();

	void render();
	void update();

	bool pollNewWindowCommand();

	const SRIP_args getSRIP_args();

private:
	bool createNewWindow = false;

	SRIP_args args;
};