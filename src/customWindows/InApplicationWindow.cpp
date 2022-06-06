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

void InApplicationWindow::shutdown(){}

string InApplicationWindow::getWindowName()
{
	return this->windowName;
}
