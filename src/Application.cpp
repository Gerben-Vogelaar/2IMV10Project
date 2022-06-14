#include "Application.h"
#include <src/newick/NewickTree.h>
#include <src/iciclePlot/SRIParg.h>
#include <src/iciclePlot/SpaceReclaimingIciclePlot.h>
#include <src/customWindows/MainDockingWindow.h>

namespace IciclePlotApp {

	void handleMouseEvents();

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
	{
		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Run()
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(Shader("resources/shaderFiles/framebuffer.vs", "resources/shaderFiles/framebuffer.fs"));
		std::shared_ptr<Shader> shader2 = std::make_shared<Shader>(Shader("resources/shaderFiles/shader_imGUIGraph.vs", "resources/shaderFiles/shader_imGUIGraph.fs"));
		//Shader shaderScreen = Shader("resources/shaderFiles/framebuffers_screen.vs", "resources/shaderFiles/framebuffers_screen.fs");

		ifstream ifile;
		//ifile.open("./resources/newickTrees/life.txt");
		ifile.open("./resources/newickTrees/test.txt");

		stringstream buf;
		buf << ifile.rdbuf();
		string as(buf.str());
		Newick newick = Newick(as);
		newick.printStatistics();

		float valueW = 1.0f;
		float hValue = 0.2f;

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


		//SpaceReclaimingIciclePlot plot = SpaceReclaimingIciclePlot(newick, args1, true, QUAD_PRECISION);

		SpaceReclaimingIciclePlot* plot = new SpaceReclaimingIciclePlot(newick, args1, false, 50);

		float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
		};

		float vertices2[] = {
		-1.0f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
		};
		
		float quad[] = {
		 0.5f,  0.5f,
		 0.5f, -0.5f,
		-0.5f,  0.5f,
		-0.5f, -0.5f
		};

		MainDockingWindow* initWindow = new MainDockingWindow();

		Scene scene(shader, sizeof(vertices), vertices, 3, m_Specification.Width, m_Specification.Height, 3, true);
		Scene scene2(shader, sizeof(vertices2), vertices2, 3, m_Specification.Width, m_Specification.Height, 3, true);

		Scene scene3(shader2, sizeof(quad), quad, 4, m_Specification.Width, m_Specification.Height, 2, true);
		scene3.setDrawMode(GL_QUADS);

		Scene scene4(shader2, plot->getVertexDataArraySize() * sizeof(float), plot->getVertexDataArray(), plot->getVertexDataArraySize(),
			m_Specification.Width, m_Specification.Height, 2, true);
		delete(plot);

		initWindow->newWindow("1");
		initWindow->newWindow("2");
		initWindow->newWindow("3");
		initWindow->newWindow("3");
		initWindow->newImageWindow("4", std::make_shared<Scene>(scene));
		initWindow->newImageWindow("5", make_shared<Scene>(scene2));
		initWindow->newImageWindow("6", make_shared<Scene>(scene3));
		initWindow->newImageWindow("7", make_shared<Scene>(scene4));
			
		ImGui::GetIO().WantCaptureMouse = true;
		ImGui::GetIO().WantCaptureKeyboard = true;

		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			initWindow->renderContent();
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO& io = ImGui::GetIO();

			handleMouseEvents();

			(void)io;
			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(window);
		}

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	


	void Application::Init()
	{
		if (m_Running) {
			std::cout << "Application already initialized" << std::endl;
			return;
		}

		if (!glfwInit())
		{
			std::cout << "ERROR initializing glfw" << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(), NULL, NULL);

		if (!window)
		{
			// Window or OpenGL context creation failed
			std::cout << "ERROR initializing glfw windows; Window or OpenGL context creation failed" << std::endl;
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (glewInit() != GLEW_OK) {
			std::cout << "Failed initializing Glew" << std::endl;
		}

		std::cout << "OpenGL version:  " << glGetString(GL_VERSION) << std::endl;

		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	
#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
		const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
		// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
		//// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		////glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		////glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;


	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void Application::Shutdown()
	{
		glfwTerminate();

		//Destroy ImGui context
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	//Handle all mouse events
	void handleMouseEvents() {
		
	}
}

