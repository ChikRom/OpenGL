#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement
{
	FORWARD, BACKWARD, LEFT, RIGHT
};

const float PITCH = 0.0f;
const float YAW = -90.0f;
const float MOUSE_SENS = 0.1f;
const float MOVE_SPEED = 20.5f;
const float FOV = 45.0f;

class Camera
{
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	float Pitch, Yaw, Fov, mouseSensitivity, moveSpeed;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float pitch = PITCH, float yaw = YAW, float mouse_sens = MOUSE_SENS, float move_speed = MOVE_SPEED,
		float fov = FOV);
	glm::mat4 GetViewMatrix();

	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMove(float xoffset, float yoffset, bool constrainPitch = true);
	void processMouseScroll(float yoffset);
private:
	void updateCameraVectors();

};

#endif