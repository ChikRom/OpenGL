#include "camera.h"





Camera::Camera(glm::vec3 position, glm::vec3 up,
	float pitch, float yaw, float mouse_sens, float move_speed,
	float fov)
	: Pos(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	WorldUp(up), Pitch(pitch), Yaw(yaw), mouseSensitivity(mouse_sens),
	moveSpeed(move_speed), Fov(fov)
{
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);

	glm::vec3 viewZ = glm::normalize(Front);
	glm::vec3 viewX = glm::normalize(glm::cross(viewZ, WorldUp));
	glm::vec3 viewY = glm::normalize(glm::cross(viewX, viewZ));

	viewMatrix[0][0] = viewX.x;
	viewMatrix[1][0] = viewX.y;
	viewMatrix[2][0] = viewX.z;
	viewMatrix[0][1] = viewY.x;
	viewMatrix[1][1] = viewY.y;
	viewMatrix[2][1] = viewY.z;
	viewMatrix[0][2] = -viewZ.x;
	viewMatrix[1][2] = -viewZ.y;
	viewMatrix[2][2] = -viewZ.z;
	viewMatrix[3][0] = glm::dot(viewX, -Pos);
	viewMatrix[3][1] = glm::dot(viewY, -Pos);
	viewMatrix[3][2] = glm::dot(-viewZ, -Pos);
	return viewMatrix;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = moveSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
		Pos += Front * velocity;
	if (direction == CameraMovement::BACKWARD)
		Pos -= Front * velocity;
	if (direction == CameraMovement::LEFT)
		Pos -= glm::normalize(glm::cross(Front, WorldUp)) * velocity;
	if (direction == CameraMovement::RIGHT)
		Pos += glm::normalize(glm::cross(Front, WorldUp)) * velocity;
}
void Camera::processMouseMove(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	Pitch += yoffset;
	Yaw += xoffset;
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	updateCameraVectors();
}
void Camera::processMouseScroll(float yoffset)
{
	Fov -= yoffset * 3.0f;
	if (Fov > 45.0f)
		Fov = 45.0f;
	if (Fov < 1.0f)
		Fov = 1.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	newFront.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	newFront.y = glm::sin(glm::radians(Pitch));
	newFront.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	Front = glm::normalize(newFront);

}