#include "imGUIWrapper.h"

imGUIWrapper::imGUIWrapper(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    //initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
}

void imGUIWrapper::renderInterface()
{
    //setup 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2{ 0.0F, 0.0F });
    //ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Settings");
    ImGui::Checkbox("Rotate plot", &rotatePlot);
    ImGui::Checkbox("Rasterize plot", &rasterize);
    ImGui::Text("");
    ImGui::Text("Set parameters:");
    //Dropdown 
    const char* items[] = { "Algorithm 1", "Algorithm 2" };
    static const char* current_item = "Select algorithm";

    if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(items[n], is_selected))
                current_item = items[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Parameters algorithm 1:");
    ImGui::SliderFloat("h", &hValue, 0.0f, 1.0f);
    ImGui::SliderFloat("W", &valueW, 0.0f, 2.0f);
    ImGui::SliderFloat("Gamma", &valueGamma, 0.0f, 0.5f);
    ImGui::SliderFloat("Rho", &valueRho, 0.0f, 1.0f);
    ImGui::Text("Extra parameters algorithm 2:");
    ImGui::SliderFloat("Epsilon", &valueEpsilon, 0.0f, valueW);
    ImGui::SliderFloat("Sigma", &valueSigma, 0.0f, 1.0f);
    ImGui::SliderFloat("Lambda", &valueLambda, 0.0f, 50.0f);
    if (ImGui::Button("Generate plot")) {

        generateNewPlot = true;

        if (current_item == "Algorithm 1") {
            SRIP1_arg args1;
            args1.setGamma(valueGamma);
            args1.seth(hValue);
            args1.setRho(valueRho);
            args1.setW(valueW);

            this->args1 = args1;

            algorithmSelected = ALGORITHM_1;
        }
        else if (current_item == "Algorithm 2") {
            SRIP2_arg args2;
            args2.setGamma(valueGamma);
            args2.seth(hValue);
            args2.setRho(valueRho);
            args2.setW(valueW);
            args2.setEpsilon(valueEpsilon);
            args2.setSigma(valueSigma);
            args2.setLambda(valueLambda);

            this->args2 = args2;

            algorithmSelected = ALGORITHM_2;
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool imGUIWrapper::pollNewPlot()
{
    if (generateNewPlot) {
        generateNewPlot = false;
        return true;
    }

    return false;
}

SRIP1_arg imGUIWrapper::getArgs1()
{
    return this->args1;
}

SRIP2_arg imGUIWrapper::getArgs2()
{
    return this->args2;
}

AlgorithmSelected imGUIWrapper::getAlgorithmSelected()
{
    return algorithmSelected;
}

bool imGUIWrapper::getRotate()
{
    return this->rotatePlot;
}

bool imGUIWrapper::getRasterize()
{
    return this->rasterize;
}
