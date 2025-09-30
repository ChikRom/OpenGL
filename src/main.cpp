#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// function which check if user pressed the escape key to close the window
void processInput(GLFWwindow* window);




//Vertex shader
const char* vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";

//Fragment shader
const char* fragmentShaderSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.3568f, 0.0f, 0.7294f, 1.0f);\n"
"}\n\0";

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

	
	//create Vertex shader
	//attach the Vertex Shader source code to the shader object
	// and compile the Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//create Fragment shader
	//attach the Fragment Shader source code to the shader object
	// and compile the Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//Create Shader program, attach our shaders to program and link them
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	//delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vertex data
	float vertices[]{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f,  0.0f
	};

	//Generate Vertex Array Object and Vertex Buffer Object
	unsigned int VBO,VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind our Vertex Array Object which will store our attributes configuration and current VBO
	glBindVertexArray(VAO);

	//bind our VBO with target buffer, send our vertex data to the VBO and specify
	// how we want to manage this data (to better allocation of memory)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set our vertex attributes for vertex shader and active vertex attribute with index 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//check if user pressed escape
		processInput(window);

		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // yellow
		//glClearColor(0.3568f, 0.0f, 0.7294f, 1.0f); // violet
		glClear(GL_COLOR_BUFFER_BIT);


		//activate the shader program, bind our VAO and draw triangle from 0-indexed vertice
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);
		// swap back and front buffers for avoiding artifacts
		// and check the events from user (mouse, keyboard)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// delete VAO, VBO and shader program
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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