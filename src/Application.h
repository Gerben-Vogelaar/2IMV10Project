#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "customWindows/InApplicationWindowImage.h"

#include <string>
#include <iostream>
#include <list>

namespace IciclePlotApp {

	struct ApplicationSpecification
	{
		std::string Name = "Walnut App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};

	class Application {

	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();

		void newWindow(string windowName);
		void newImageWindow(string windowName, unsigned int textureBuffer);

	private:
		void Init();
		void Shutdown();
		
		void renderSubWindows();

	private:
		ApplicationSpecification m_Specification;
		GLFWwindow* window = nullptr;
		
		std::list<InApplicationWindow*> subWindows;

		bool m_Running = false;
	};

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}


