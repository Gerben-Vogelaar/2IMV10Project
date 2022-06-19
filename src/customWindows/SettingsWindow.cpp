#include "settingsWindow.h"

SettingsWindow::SettingsWindow() : InApplicationWindow("Settings Panel") {
	args.setGamma(0.025f);
	args.seth(0.25f);
	args.setRho(0.8f);
	args.setW(2.0f);

	args.setEpsilon(2.0f);
	args.setSigma(1.0f);
	args.setLambda(30);
}

void SettingsWindow::render()
{
	ImGui::Begin(getWindowName().c_str(), getRunning());
	ImGui::Text("Create new icicle plot graph.");

	static float drag_f = 0.5f;
	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

	//Link each of these elements to the correct SRIP arguments
	ImGui::Text("Underlying float value: %f", drag_f);
	ImGui::DragFloat("W", &args.W, 0.005f, 0.0f, 2.0f, "%.3f", flags);
	ImGui::DragFloat("h", &args.h, 0.005f, 0.01f, 1.0f, "%.3f", flags);
	ImGui::DragFloat("gamma", &args.gamma, 0.005f, 0.0f, 1.0f, "%.3f", flags);
	ImGui::DragFloat("rho", &args.rho, 0.005f, 0.0f, 1.0f, "%.3f", flags);
	ImGui::DragFloat("epsilon", &args.epsilon, 0.005f, 0.0f, args.W, "%.3f", flags);
	ImGui::DragFloat("sigma", &args.sigma, 0.005f, 0.0f, 1.0f, "%.3f", flags);
	ImGui::DragFloat("lambda", &args.lambda, 0.005f, 0.0f, 100.0f, "%.3f", flags);
	
	const char* items[] = { AlgorithmTypeToString(AlgorithmType::SRIP1), AlgorithmTypeToString(AlgorithmType::SRIP2) };
	int elementsItems = 2;
	static int item_current = 1;

	ImGui::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), elementsItems); 
	args.setAlgoType(StringToAlgorithmType(items[item_current]));

	if (ImGui::Button("Generate new Window", ImVec2(200.0f, 40.0f))) {
		args.printArgs();
		createNewWindow = true;
	};

	ImGui::End();
}

void SettingsWindow::update() {}

/* return True if we need to create a new window, will set the bool to false subsequently.
*/
bool SettingsWindow::pollNewWindowCommand()
{
	if (createNewWindow) {
		createNewWindow = false;
		return true;
	}

	return false;
}

const SRIP_args SettingsWindow::getSRIP_args() {
	return args;
}