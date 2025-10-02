#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// function which check if user pressed the escape key to close the window
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
	GLFWwindow* window = glfwCreateWindow(1600, 1000, "OpenGL", NULL, NULL);

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

	// Create our vertex and fragment shader, compile them and link into one shader programm
	Shader ourShader("shaders/vertex.vs", "shaders/fragment.fs");

	//vertex data
	//float vertices[]
	//{
	//	-0.5f, -0.5f, 0.0f,
	//	 0.5f, -0.5f, 0.0f,
	//	 0.5f,  0.5f, 0.0f,
	//	//-0.5f,  0.5f, 0.0f
	//};

	float vertices[]
	{	// vertices				// colors
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f
	};

	// indices data
	unsigned int indices[] =
	{
		0,1,2,
		//0,3,2
	};

	//Generate Vertex Array Object, Vertex Buffer Object and Element Buffer Object
	unsigned int VBO,VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//bind our Vertex Array Object which will store our attributes configuration and current VBO
	glBindVertexArray(VAO);

	//bind our VBO with target buffer, send our vertex data to the VBO and specify
	// how we want to manage this data (to better allocation of memory)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind our EBO with target buffer, send our vertex data to the VBO and specify
	// how we want to manage this data (to better allocation of memory)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// set our vertex and colour attributes for vertex shader and active vertex attribute with index 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind VAO, VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	float offset = -0.5f;
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//check if user pressed escape
		processInput(window);

		if (offset > 0.5f)
			offset = -0.5f;
		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // yellow
		//glClearColor(0.3568f, 0.0f, 0.7294f, 1.0f); // violet
		glClear(GL_COLOR_BUFFER_BIT);
		//activate the shader program
		ourShader.use();
		/*offset+=0.00003f;
		ourShader.setFloat("offset", offset);*/

		//ourShader.setFloat("offset", offset);
		//bind our VAO and draw triangle from 0-indexed vertices
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		// swap back and front buffers for avoiding artifacts
		// and check the events from user (mouse, keyboard)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// delete VAO, VBO, EBO and shader program
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);
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
		glfwSetWindowShouldClose(window, true); // set the value of the close flag of the specified window
}