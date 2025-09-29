#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// funcion which check if user pressed the escape key to close the window
void processInput(GLFWwindow* window);

int main()
{
	//initialize the GLFW library
	glfwInit();
	// set the version and add core features for GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the context of our window current on the calling thread
	glfwMakeContextCurrent(window);

	// initialize GLAD to determine the functions and manage function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// tell GLFW to call the function framebuffer_size_callback on every window
	// resize by registering it:
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//check if user pressed escape
		processInput(window);

		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(0.3568f, 0.0f, 0.7294f, 1.0f); // violet
		glClear(GL_COLOR_BUFFER_BIT);



		// swap back and front buffers for avoiding artifacts
		// and check the events from user (mouse, keyboard)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}





void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// tell OpenGL the size of the rendering window
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); // set the value of the close flag of the specified
}