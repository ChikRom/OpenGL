#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"

// callback function which executes every time when the window size is change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// callbacck function which executes every time when we move our mouse
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

// callback function which executes every time when we scroll our mouse wheel
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// function which check if user pressed the escape key to close the window
void processInput(GLFWwindow* window);

//unsigned int loadTexture(char const* path);

// Global varibles
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // camera object
bool firstMouse = true; // for capture mouse in different places on the screen
float lastX = 2400.0f / 2.0f; // x-centre of the screen for capturing mouse
float lastY = 1200.0f / 2.0f; // y-centre of the screen for capturing mouse
float deltaTime = 0.0f; // time difference between render current and last frames
float lastFrame = 0.0f; // time for render last frame

//glm::vec3 LightPos(1.2f,1.0f,2.0f);

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


	// vertices for cube
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	};

	unsigned int lightVAO, lightVBO;

	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,lightVBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	glEnable(GL_DEPTH_TEST);
	Shader lightShader("shaders/vertexLight.glsl", "shaders/fragmentLight.glsl");

	Shader ourShader("shaders/vertexModel.glsl", "shaders/fragmentModel.glsl");

	Model ourModel("models/banana/Banana.obj");
	
	ourShader.use();

	glm::vec3 dirlightDirection = glm::vec3(1.5f, 1.0f, 1.0f);
	glm::vec3 dirlightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 dirlightDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	glm::vec3 dirlightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);


	glm::vec3 pointLightPosition = glm::vec3(3.0, 3.0f, 3.5f);
	glm::vec3 pointLightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 pointLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 pointLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);


	glm::vec3 spotlightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 spotlightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 spotlightDiffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	glm::vec3 spotlightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

	float pointConstant = 1.0f;
	float pointLinear = 0.09f;
	float pointQuadratic = 0.032f;

	ourShader.setVec3("dirLight.ambient", dirlightAmbient);
	ourShader.setVec3("dirLight.diffuse", dirlightDiffuse);
	ourShader.setVec3("dirLight.specular", dirlightSpecular);

	ourShader.setFloat("pointLight.constant", pointConstant);
	ourShader.setFloat("pointLight.linear", pointLinear);
	ourShader.setFloat("pointLight.quadratic", pointQuadratic);

	ourShader.setVec3("pointLight.ambient", pointLightAmbient);
	ourShader.setVec3("pointLight.diffuse", pointLightDiffuse);
	ourShader.setVec3("pointLight.specular", pointLightSpecular);

	ourShader.setFloat("spotLight.constant", pointConstant);
	ourShader.setFloat("spotLight.linear", pointLinear);
	ourShader.setFloat("spotLight.quadratic", pointQuadratic);

	ourShader.setVec3("spotLight.ambient", spotlightAmbient);
	ourShader.setVec3("spotLight.diffuse", spotlightDiffuse);
	ourShader.setVec3("spotLight.specular", spotlightSpecular);

	ourShader.setFloat("spotLight.inCutOff", glm::cos(glm::radians(10.0f)));
	ourShader.setFloat("spotLight.outCutOff", glm::cos(glm::radians(12.5f)));

	float shininess = 16.0f;

	ourShader.setFloat("shininess", shininess);
	float radius = 2.5f;
	//render loop
	while (!glfwWindowShouldClose(window))
	{


		// calculate the deltatime between current and last frame
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//const float radius = 3.0f;
		float lightX = sin(glfwGetTime()*1.5) * radius;
		float lightZ = cos(glfwGetTime()*1.5) * radius;

		//LightPos.x = lightX;
		//LightPos.z = lightZ;

		//check if user pressed escape or move the camera
		processInput(window);

		glm::mat4 view = camera.getViewMatrix();


		// set clear values for the colour buffers
		// and clear buffers to preset values
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // yellow

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		ourShader.use();

		ourShader.setVec3("dirLight.direction", glm::vec3(view * glm::vec4(dirlightDirection, 0.0f)));


		ourShader.setVec3("pointLight.position", glm::vec3(view * glm::vec4(pointLightPosition, 1.0f)));

		ourShader.setVec3("spotLight.direction", spotlightDirection);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.zoom), 2400.0f / 1200.0f, 0.1f, 100.0f);
		
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(lightX, 0.0f, lightZ));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		ourShader.setMat4("model", model);
		
		ourModel.Draw(ourShader);


		lightShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPosition);
		model = glm::scale(model, glm::vec3(0.3f));

		lightShader.setMat4("model", model);
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// swap back and front buffers for avoiding artifacts
		// and check the events from user (mouse, keyboard)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// delete VAO, VBO
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);

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



//unsigned int loadTexture(char const* path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}