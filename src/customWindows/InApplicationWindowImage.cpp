#include "InApplicationWindowImage.h"

InApplicationWindowImage::InApplicationWindowImage(string windowName, unsigned int textureBuffer) : textureBuffer(textureBuffer), InApplicationWindow(windowName)
{
}

void InApplicationWindowImage::render()
{
	ImGui::Begin(getWindowName().c_str());

	// Get the current cursor position (where your window is)
	ImVec2 pos = ImGui::GetCursorScreenPos();
	unsigned int tex = textureBuffer;

	float h = 0, w = 0;

	ImVec2 windowSize = ImGui::GetWindowSize();

	ImVec2 posCursor = ImGui::GetCursorScreenPos();
	ImVec2 maxPos = ImVec2(pos.x + ImGui::GetWindowSize().x, pos.y + ImGui::GetWindowSize().y);

	ImGui::GetWindowDrawList()->AddImage(
		(void*)tex,
		ImVec2(posCursor.x, posCursor.y),
		ImVec2(maxPos),
		ImVec2(0, 1), ImVec2(1, 0)
	);

	ImGui::End();

}

void InApplicationWindowImage::addTextureBuffer(unsigned int textureBuffer)
{
	this->textureBuffer = textureBuffer;
}
