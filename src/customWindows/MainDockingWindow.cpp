#include "MainDockingWindow.h"

MainDockingWindow::MainDockingWindow()
{
    
}

MainDockingWindow::~MainDockingWindow() {
	for (InApplicationWindow* subWindow : subWindows) {
		free(subWindow);
	}
}

void MainDockingWindow::renderContent()
{
    bool running = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &running, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        std::cout << "SHOW DOCKING DISABLE MESSAGE" << std::endl;
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, running != NULL))
                running = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    update();

    ImGui::End();
}

void MainDockingWindow::newWindow(string windowName)
{

	for (InApplicationWindow* subWindow : subWindows) {
		if (subWindow->getWindowName() == windowName) {
			std::cout << "Cannot create another window with the name:" + windowName + " - ImGui will combine similar named windows" << std::endl;
			return;
		}
	}

	subWindows.push_back(new InApplicationWindow(windowName));
}

void MainDockingWindow::newImageWindow(string windowName, std::shared_ptr<Scene> scene)
{
	for (InApplicationWindow* subWindow : subWindows) {
		if (subWindow->getWindowName() == windowName) {
			std::cout << "Cannot create another window with the name:" + windowName + " - ImGui will combine similar named windows" << std::endl;
			return;
		}
	}

	subWindows.push_back(new InApplicationWindowImage(windowName, scene));
}

void MainDockingWindow::newImageWindowGraph(string windowName)
{
    //TODO: should retrieve args from the settings window

    std::ifstream ifile;
    ifile.open("./resources/newickTrees/life.txt");

    std::stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());
    Newick newick = Newick(as);
    newick.printStatistics();

    SRIP1_arg args1;
    args1.setGamma(0.025f);
    args1.seth(0.25f);
    args1.setRho(0.8f);
    args1.setW(2.0f);

    SRIP2_arg args2;
    args2.setGamma(0.1f);
    args2.seth(0.085f);
    args2.setRho(0.1f);
    args2.setW(2.0f);
    args2.setEpsilon(2.0f);
    args2.setSigma(1.0f);
    args2.setLambda(30);

    SpaceReclaimingIciclePlot* plot = new SpaceReclaimingIciclePlot(newick, args1, false, 50);

    std::shared_ptr<Shader> shader2 = std::make_shared<Shader>(Shader("resources/shaderFiles/shader_imGUIGraph.vs", "resources/shaderFiles/shader_imGUIGraph.fs"));
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Scene(shader2, plot->getVertexDataArraySize() * sizeof(float), plot->getVertexDataArray(), plot->getVertexDataArraySize(),
        1600, 900, 2, true));

    newImageWindow("New name", scene);
}

string MainDockingWindow::generateName(string windowName, int count)
{
    string countString = std::to_string(count);

    if (count == 0) {
        countString = "";
    }

    size_t lastindex = windowName.find_last_of(".");
    string rawname = windowName.substr(0, lastindex);
    string extension = windowName.substr(lastindex, windowName.length());
    string currentName = rawname + countString + extension;

    for (InApplicationWindow* win : subWindows) {
        if (win->getWindowName().compare(currentName) == 0) {
            return generateName(windowName, count + 1);
        }
    }

    return currentName;
}

void MainDockingWindow::newImageWindowGraph2(std::filesystem::path filePath, SRIP_args arg) {
    std::ifstream ifile;
    ifile.open(filePath);
    std::stringstream buf;
    buf << ifile.rdbuf();
    string as(buf.str());
    Newick newick = Newick(as);
    newick.printStatistics();
   
    std::shared_ptr<Shader> shader2 = std::make_shared<Shader>(Shader("resources/shaderFiles/shader_imGUIGraph.vs", "resources/shaderFiles/shader_imGUIGraph.fs"));
    std::shared_ptr<Scene> scene;

    if (arg.algo == AlgorithmType::SRIP1) {
        SpaceReclaimingIciclePlot* plot = new SpaceReclaimingIciclePlot(newick, arg.getSRIP1arg(), false, 50);
        scene = std::make_shared<Scene>(Scene(shader2, plot->getVertexDataArraySize() * sizeof(float), plot->getVertexDataArray(), plot->getVertexDataArraySize(),
            1600, 900, 2, true));
    } else if (arg.algo == AlgorithmType::SRIP2) {
        SpaceReclaimingIciclePlot* plot = new SpaceReclaimingIciclePlot(newick, arg.getSRIP2arg(), false, 50);
        scene = std::make_shared<Scene>(Scene(shader2, plot->getVertexDataArraySize() * sizeof(float), plot->getVertexDataArray(), plot->getVertexDataArraySize(),
            1600, 900, 2, true));
    }

    string graphWindowName = generateName(filePath.filename().string(), 0);

    newImageWindow(graphWindowName, scene);
}

void MainDockingWindow::pollSubwindows()
{
    if (settingsWindow.get()->pollNewWindowCommand()) {
 
        SRIP_args args = settingsWindow.get()->getSRIP_args();
        
        newImageWindowGraph2(std::filesystem::path("./resources/newickTrees/life.txt"), args);
    }
}

void MainDockingWindow::update()
{
    pollSubwindows();

    settingsWindow.get()->render();

    for (InApplicationWindow* window : subWindows) {
        window->update();
        window->render();
    }
}
