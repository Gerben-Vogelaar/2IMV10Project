#pragma once

#include "InApplicationWindow.h"

#include "GL/glew.h"
#include "src/shader/shader.h"

#include <iostream>



	class InApplicationWindowImage : public InApplicationWindow{

	public:
		InApplicationWindowImage(string windowName, unsigned int textureBuffer);

		void render();
		void addTextureBuffer(unsigned int textureBuffer);

	private:
		unsigned int textureBuffer = NULL;
	};

