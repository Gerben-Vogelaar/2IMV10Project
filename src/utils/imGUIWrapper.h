#pragma once

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include "src/iciclePlot/SRIParg.h"

#include <iostream>

enum AlgorithmSelected { ALGORITHM_1, ALGORITHM_1_E, ALGORITHM_2 };

class imGUIWrapper
{

public:
	imGUIWrapper(GLFWwindow* window);
	
	void renderInterface();
	bool pollNewPlot();

	SRIP1_arg getArgs1();
	SRIP2_arg getArgs2();
	AlgorithmSelected getAlgorithmSelected();
	bool getRotate();
	bool getRasterize();

private:
	AlgorithmSelected algorithmSelected = ALGORITHM_1;

	SRIP1_arg args1 = SRIP1_arg();
	SRIP2_arg args2 = SRIP2_arg();

	float valueW = 2.0f;
	float hValue = 0.085f;
	float valueRho = 0.4f;
	float valueGamma = 0.1f;
	float valueEpsilon = 2.0f;
	float valueSigma = 1.0f;
	float valueLambda = 30;

	bool rotatePlot = false;
	bool rasterize = false;

	bool generateNewPlot = false;
};

