#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <map>;
#include "DebugOutput.h"
#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "model.h"
#include <vector>
#include "shadow_map_fbo.h"

glm::mat4 projection;

float deltaTime = 0.0f;
float SCREEN_WIDTH = 3200.0f;
float SCREEN_HEIGHT = 2400.0f;
float pitch = 0.0f;
float yaw = -90.0f;
float fov = 45.0f;
bool firstMouse = true;
double lastX = 960 / 2.0f;
double lastY = 540 / 2.0f;
bool blinn = false;
bool blinnKeyPressed = false;
bool shadowsKeyPressed = false;
bool SRGB = false;
bool SRGB_pressed = false;
bool shadows = true;
bool kindOfLight = false;
bool kindOfLightPressed = false;

// camera
Camera camera;

void allignObject(Model& model, Shader& shader, glm::mat4& modelMatrix);
void renderQuad();
void renderScene1(const Shader& shader);
void renderScene2(const Shader& shader);

uint32_t loadTexture(char const* path);
uint32_t loadCubeMap(std::vector<std::string>& faces, const std::string& path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void ShadowMapPass(ShadowMapFBO& shadowFBO, Shader& shader);
void LightingPass(ShadowMapFBO& shadowFBO, Shader& shader);

uint32_t planeVAO;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPENGL", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GFLW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	setDebugOutput();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetWindowPos(window, 1920, -400);


	// for delta time
	float lastFrame = 0.0f;
	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
	glActiveTexture(GL_TEXTURE0);
	uint32_t diffuseMap = loadTexture(RESOURCE_PATH "brickwall.jpg");
	glActiveTexture(GL_TEXTURE1);
	uint32_t normalMap = loadTexture(RESOURCE_PATH "brickwall_normal.jpg");

	Shader pointLightShader(SHADER_PATH "pointLight.vert", SHADER_PATH "pointLight.frag");
	//pointLightShader.setInt("diffuseMap", 0);
	//pointLightShader.setInt("normalMap", 1);
	pointLightShader.setVec3("lightPos", lightPos);
	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);
	Model backpack(RESOURCE_PATH "backpack/backpack.obj", true);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		pointLightShader.setMat4("projection", projection);
		pointLightShader.setMat4("view", view);
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		pointLightShader.setMat4("model", model);
		pointLightShader.setVec3("viewPos", camera.Pos);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		renderQuad();*/
		backpack.Draw(pointLightShader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}


void ShadowMapPass(ShadowMapFBO& shadowFBO, Shader& shader)
{
	shadowFBO.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene1(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void LightingPass(ShadowMapFBO& shadowFBO, Shader& shader)
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	shadowFBO.BindForReading(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene1(shader);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	projection = glm::perspective(camera.Fov, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	// camera buttons input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::FORWARD,deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::LEFT,deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !blinnKeyPressed)
	{
		blinn = !blinn;
		blinnKeyPressed = true;
		std::cout << "Change to "<< (blinn == true ? "Blinn-Phong" : "Phong") << " Lighting\n";
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
	{
		blinnKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !shadowsKeyPressed)
	{
		shadows = !shadows;
		shadowsKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		shadowsKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !kindOfLightPressed)
	{
		kindOfLight = !kindOfLight;
		kindOfLightPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		kindOfLightPressed = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double offsetX = xpos - lastX;
	double offsetY = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMove(offsetX, offsetY);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

uint32_t loadTexture(char const* path)
{
	uint32_t textureID;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}

void allignObject(Model& model, Shader& shader, glm::mat4& modelMatrix)
{
	glStencilMask(0xFF);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.03f));
	shader.setMat4("model", modelMatrix);
	shader.use();
	model.Draw(shader);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
}

uint32_t loadCubeMap(std::vector<std::string>& faces, const std::string& path)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrComponents;
	for (uint32_t i = 0; i < faces.size(); i++)
	{
		uint8_t* data = stbi_load((path+ faces[i]).c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "ERROR::TEXTURE:: Failed to load CubeMap at path: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void renderScene1(const Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Cube textureCube(NORMAL_TEXTURE_CUBE);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 2.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);
}

void renderScene2(const Shader& shader)
{

	Cube textureCube(NORMAL_TEXTURE_CUBE);
	glm::mat4 model = glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	shader.setMat4("model", model);
	glDisable(GL_CULL_FACE);
	shader.setBool("reverse_normals", 1);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);
	shader.setBool("reverse_normals", 0);
	glEnable(GL_CULL_FACE);



	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(0.75));
	shader.setMat4("model", model);
	glBindVertexArray(textureCube.VAO);
	glDrawElements(GL_TRIANGLES, textureCube.numIndices, GL_UNSIGNED_INT, 0);
}