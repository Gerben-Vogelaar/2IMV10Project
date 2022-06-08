#pragma once


#include "Scene.h"
#include <list>

class SceneHandler {

public:
	SceneHandler();
	~SceneHandler();

	void updateScenes();
	void addScene(Scene* scene);

private:
	std::list<Scene*> scenes;
};