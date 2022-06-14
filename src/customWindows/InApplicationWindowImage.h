#pragma once

#include "InApplicationWindow.h"

#include "GL/glew.h"
#include "src/shader/shader.h"

#include <iostream>
#include <src/SceneUtils/Scene.h>



	class InApplicationWindowImage : public InApplicationWindow{

	public:
		InApplicationWindowImage(string windowName, std::shared_ptr<Scene> scene);

		void render();
		void update();
		
	private:
		std::shared_ptr<Scene> scene;

		ImVec2 previousDraggedDistance = ImVec2(0.0f, 0.0f);
		ImVec2 draggedDistance = ImVec2(0.0f, 0.0f);
		bool dragging = false;
		const float MOVE_SPEED_IMAGE = 0.01f;

		void handleInput();
	};

