#include "Camera.h"

//Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	vec3 front;
	front.x = cos(radians(this->Yaw)) * cos(radians(this->Pitch));
	front.y = sin(radians(this->Pitch));
	front.z = sin(radians(this->Yaw)) * cos(radians(this->Pitch));
	this->Front = normalize(front);
	//Also re-calculate the Right and Up vector
	this->Right = normalize(cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = normalize(cross(this->Right, this->Front));
}

//Constructor with vectors
Camera::Camera(vec3 position, vec3 up, GLfloat yaw, GLfloat pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.25f), Zoom(45.0f)
{
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

//Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.25f), Zoom(45.0f)
{
	this->Position = vec3(posX, posY, posZ);
	this->WorldUp = vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

//Returns the view matrix calculated using Eular Angles and the LookAt Matrix
mat4 Camera::GetViewMatrix()
{
	return lookAt(this->Position, this->Position + this->Front, this->WorldUp);
}

//Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

//Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	//Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	//Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

//Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

Camera::~Camera()
{

}