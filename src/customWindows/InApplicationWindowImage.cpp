#include "InApplicationWindowImage.h"

InApplicationWindowImage::InApplicationWindowImage(string windowName, std::shared_ptr<Scene> scene) : scene(scene), InApplicationWindow(windowName)
{}

void InApplicationWindowImage::render()
{

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(getWindowName().c_str());	
	

	// Get the current cursor position (where your window is)
	ImVec2 pos = ImGui::GetCursorScreenPos();
	unsigned int tex = scene->getTextureBuffer();

	float h = 0, w = 0;

	//ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	/*cout << getWindowName() << endl; 
	cout << "( " << windowSize.x << "," << windowSize.y << ") " << endl;
	cout << "( " << ImGui::GetWindowSize().x << "," << ImGui::GetWindowSize().y << ") " << endl;*/

	//ImVec2 posCursor = ImGui::GetCursorScreenPos();
	ImVec2 maxPos = ImVec2(pos.x + windowSize.x, pos.y + windowSize.y);

	 ImGui::GetWindowDrawList()->AddImage(
		(void*)tex,
		//ImVec2(0, 0),
		ImGui::GetWindowPos(),
		maxPos,
		 //ImVec2(maxPos.x * c, maxPos.y * c),
		ImVec2(0, 1), ImVec2(1, 0)
	);

	handleInput();

	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void InApplicationWindowImage::update()
{
	scene->setViewport(0, 0, (int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y);
	scene->updateScene();
}

void InApplicationWindowImage::handleInput()
{

	if (ImGui::IsWindowHovered()) {
		if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left, false)) {
			//scene.get()->cameraDebug();
		}

		//handle scrolling of the image
		if (ImGui::GetIO().MouseWheel) {
			scene.get()->zoomScene(ImGui::GetIO().MouseWheel);
		}

		//handle dragging mouse of the window
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !dragging) {
			previousDraggedDistance = ImGui::GetMouseDragDelta();
			dragging = true;
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			dragging = false;
		}
	}

	if (dragging) {
		if (!ImGui::IsWindowHovered()) {
			dragging = false;
			return;
		}
		draggedDistance = ImGui::GetMouseDragDelta();

		glm::vec2 deltaChangeWindowPos = glm::vec2(previousDraggedDistance.x - draggedDistance.x, previousDraggedDistance.y - draggedDistance.y);
		
		scene.get()->moveScene(deltaChangeWindowPos * MOVE_SPEED_IMAGE);
		previousDraggedDistance = draggedDistance;
	}
}
