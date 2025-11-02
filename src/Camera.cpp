#include "Camera.h"


// create the camera
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float moveSpeed, float mouseSense, float zoom)
{
    Position = position;
    Front = front;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    movementSpeed = moveSpeed;
    mouseSensitivity = mouseSense;
    this->zoom = zoom;
    updateCameraVectors();
}

// Get the view matrix every time when we change the position of camera or direction of camera point to 
glm::mat4 Camera::getViewMatrix()
{

    return glm::lookAt(Position, Position + Front, Up);
}

// move the position of the camera
void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
}


// set new Euler's angles for camera
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    Yaw += xoffset;
    Pitch += yoffset;
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

// change the zoom of camera
void Camera::processMouseScroll(float yoffset)
{
    zoom -= yoffset*2;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

// update the direction that the camera point to
void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
