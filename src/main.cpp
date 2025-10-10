#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// function which check if user pressed the escape key to close the window
void processInput(GLFWwindow* window);



float seeing = 0.2f;

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

	float vertices[]
	{	// vertices				// colors			// textures
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f,	0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	// indices data
	unsigned int indices[] =
	{
		0,1,2,
		0,3,2
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


	// set our vertex, colour and texture attributes for vertex shader and active vertex attribute with index 0,1,2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind VAO, VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// load and create texture1 and texture 2
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// tell stb_image.h to flip loaded texture's on the y-axis
	stbi_set_flip_vertically_on_load(true);

	// load image, create textures, generate mipmaps

	// first texture (WOOD)
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/woodGate.jpg", &width, &height, &nrChannels,0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load WOOD texture" << std::endl;
	}
	// free image memory
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// second texture (CAKE)
	data = stbi_load("textures/Cake.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load CAKE texture" << std::endl;
	}

	stbi_image_free(data);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set up our two uniforms (inputs) for textures for fragment shader using texture units zero and one
	ourShader.use();
	ourShader.setInt("texObject1", 0);
	ourShader.setInt("texObject2", 1);


	// create our transform matrix, translate the container to top right and rotate it in real time
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1600.0f / 800.0f, 0.1f, 100.0f);


	// set our tranformation matrix uniform
	unsigned int modelUniformLoc = glGetUniformLocation(ourShader.ID, "model");
	glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewUniformLoc = glGetUniformLocation(ourShader.ID, "view");
	glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projectionUniformLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projectionUniformLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//check if user pressed escape
		processInput(window);


		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f); // yellow
		//glClearColor(0.3568f, 0.0f, 0.7294f, 1.0f); // violet
		glClear(GL_COLOR_BUFFER_BIT);


		//activate texture units for two textures and bind each texture for each texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.setFloat("maxValue", seeing);
		//activate the shader program and draw texture rectangle
		ourShader.use();

		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);





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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		seeing += 0.0001f;
		if (seeing >= 1.0f)
		{
			seeing = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		seeing -= 0.0001f;
		if (seeing <= 0.0f)
		{
			seeing = 0.0f;
		}
	}
}