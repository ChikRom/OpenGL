#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// callbacck function which executes every time when we move our mouse
void mouseCallback(GLFWwindow* windwo, double xPos, double yPos);

// callback function which executes every time when we scroll our mouse wheel
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// function which check if user pressed the escape key to close the window
void processInput(GLFWwindow* window);

unsigned int loadTexture(char const* path);

// Global varibles
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // camera object
bool firstMouse = true; // for capture mouse in different places on the screen
float lastX = 2400.0f / 2.0f; // x-centre of the screen for capturing mouse
float lastY = 1200.0f / 2.0f; // y-centre of the screen for capturing mouse
float deltaTime = 0.0f; // time difference between render current and last frames
float lastFrame = 0.0f; // time for render last frame

glm::vec3 LightPos(1.2f,1.0f,2.0f);

int main()
{
	//initialize the GLFW library
	glfwInit();
	// set the version and add core features for GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create the window
	GLFWwindow* window = glfwCreateWindow(2400,1200 , "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the context of our window current on the calling thread
	glfwMakeContextCurrent(window);
	// tell GLFW to call the function framebuffer_size_callback on every window
	// resize by registering it:
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// tell GLFW to call these two functions every time when we move/rotate camera or use zoom
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// tell GFLW to capture out mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// initialize GLAD to determine the functions and manage function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Create our vertex and fragment shaders of cube and light, compile them and link into one shader programm
	Shader ObjectShader("shaders/vertexObject.glsl", "shaders/fragmentObject.glsl");
	Shader LightShader("shaders/vertexLight.glsl", "shaders/fragmentLight.glsl");



	// vertices for cube
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	


	//Generate Vertex Array Object, Vertex Buffer Object and Element Buffer Object
	unsigned int VBO, CubeVAO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &VBO);


	//bind our Vertex Array Object which will store our attributes configuration and current VBO
	glBindVertexArray(CubeVAO);


	//bind our VBO with target buffer, send our vertex data to the VBO and specify
	// how we want to manage this data (to better allocation of memory)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	// set our vertex, colour attributes for vertex shader and active vertex attribute with index 0,1,2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//unbind VAO, VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//create Light Vertex Array Object and bind it with the VBO
	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glEnable(GL_DEPTH_TEST); // activate z-buffer for avoiding rerender

	// colour for object and light

	//glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	float materialShininess = 64.0f;
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

	ObjectShader.use();

	//ObjectShader.setVec3("material.specular", materialSpecular);
	ObjectShader.setFloat("material.shininess", materialShininess);

	

	ObjectShader.setVec3("light.ambient", lightAmbient);
	ObjectShader.setVec3("light.diffuse", lightDiffuse);
	ObjectShader.setVec3("light.specular", lightSpecular);



	unsigned int diffuseMap = loadTexture("textures/container3D.png");
	unsigned int specularMap = loadTexture("textures/container3DSpecular.png");
	ObjectShader.setInt("material.diffuse", 0);
	ObjectShader.setInt("material.specular", 1);

	ObjectShader.setFloat("light.constant", 1.0f);
	ObjectShader.setFloat("light.linear", 0.09f);
	ObjectShader.setFloat("light.quadratic", 0.032f);
	

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//render loop
	while (!glfwWindowShouldClose(window))
	{


		// calculate the deltatime between current and last frame
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//const float radius = 3.0f;
		//float lightX = sin(glfwGetTime()*0.3) * radius;
		//float lightZ = cos(glfwGetTime()*0.3) * radius;

		//LightPos.x = lightX;
		//LightPos.z = lightZ;

		//check if user pressed escape or move the camera
		processInput(window);

		ObjectShader.use();



		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // yellow

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the transformation matrices
		glm::mat4 view = camera.getViewMatrix();
		//ObjectShader.setVec3("light.position", glm::vec3(view* glm::vec4(LightPos, 1.0f)));
		ObjectShader.setVec3("light.direction", glm::vec3(view * glm::vec4(camera.Front,0.0f)));
		ObjectShader.setFloat("light.inCutOff", glm::cos(glm::radians(12.5f)));
		ObjectShader.setFloat("light.outCutOff", glm::cos(glm::radians(17.5f)));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.zoom), 2400.0f / 1200.0f, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		// send our transformation matrices to the vertex shader as a uniforms
		ObjectShader.setMat4("projection", projection);
		ObjectShader.setMat4("view", view);
		ObjectShader.setMat4("model", model);




		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);


		// draw objectCube
		/*glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/


		glBindVertexArray(CubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ObjectShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// set the light and draw it
		/*LightShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, LightPos);
		model = glm::scale(model, glm::vec3(0.3f));

		LightShader.setMat4("projection", projection);
		LightShader.setMat4("view", view);
		LightShader.setMat4("model", model);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/




		// swap back and front buffers for avoiding artifacts
		// and check the events from user (mouse, keyboard)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// delete VAO, VBO
	glDeleteVertexArrays(1,&CubeVAO);
	glDeleteVertexArrays(1,&LightVAO);
	glDeleteBuffers(1, &VBO);
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

	// for moving the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, deltaTime);
	}
}

// for rotating the camera
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse == true)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	
	camera.processMouseMovement(xoffset, yoffset);
}

// for Camera zoom
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}



unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}