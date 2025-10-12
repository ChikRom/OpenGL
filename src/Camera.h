#ifndef CAMERA_H
#define CAMERA_H

#include <GLAD/glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//default camera attributes
const glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float PITCH = 0.0f;
const float YAW = -90.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// directions of movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Abstract camera class
class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 position = POSITION, glm::vec3 front = FRONT, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH, float moveSpeed = SPEED, float mouseSense = SENSITIVITY, float zoom = ZOOM);
	glm::mat4 getViewMatrix();
	void processKeyboard(Camera_Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void processMouseScroll(float yoffset);
private:
	void updateCameraVectors();
};


#endif
