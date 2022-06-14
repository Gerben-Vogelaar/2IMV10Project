#include "InApplicationWindow.h"

InApplicationWindow::InApplicationWindow(string windowName) : windowName(windowName), running(true)
{
}

void InApplicationWindow::render()
{
	ImGui::Begin(windowName.c_str(), &running);
	ImGui::Text("This is some useful text.");
	ImGui::End();
}

void InApplicationWindow::update() {};

void InApplicationWindow::shutdown(){}

string InApplicationWindow::getWindowName()
{
	return this->windowName;
}

glm::vec2 InApplicationWindow::getDimensions()
{
	return glm::vec2(width, height);
}
