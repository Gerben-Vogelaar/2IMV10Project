#include "SceneHandler.h"

SceneHandler::SceneHandler()
{
}

SceneHandler::~SceneHandler()
{
	for (Scene* scene : scenes) {
		delete(scene);
	}

	std::cout << "DELTED SCENEHANDLER!!" << std::endl;

}

void SceneHandler::updateScenes()
{
	for (Scene* scene : scenes) {
		scene->updateScene();
	}
}

void SceneHandler::addScene(Scene* scene)
{
	scenes.push_back(scene);

}
