#include "../include/camera.h"


//constructor with vectors
Camera::Camera(glm::vec3 position,
	glm::vec3 up,
	float yaw,
	float pitch)
	: 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM) 
{

	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVectors();
}

//constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM) 
{

	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);

	Yaw = yaw;
	Pitch = pitch;

	updateCameraVectors();

}



//return the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

//processes input received from any keyboard-like unput sytstem.
//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, double deltaTime) {

	float velocity = (float)(MovementSpeed * deltaTime);
	if (direction == Camera_Movement::FORWARD) {
		Position += Front * velocity;
	}
	else if (direction == Camera_Movement::BACKWARD) {
		Position -= Front * velocity;
	}
	else if (direction == Camera_Movement::LEFT) {
		Position -= Right * velocity;
	}
	else if (direction == Camera_Movement::RIGHT) {
		Position += Right * velocity;
	}
}

//processes input received from a mouse input system.
//Expects the offset value in both the x and y direction
/**
* 监听鼠标移动事件时调用此方法， 控制镜头的移动
*
* 调用此方法前，
* 1. 确保设置获取光标，隐藏光标的方法：
*		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
* 2. 并且注册鼠标移动事件的回调函数:
*		glfwSetCursorPosCallback(window, mouse_callback);
* 3. 在鼠标移动事件的回调函数中调用此函数
*		void mouse_callback(GLFWwindow * window, double xpos, double ypos){  //... }
*/
void Camera::ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch) {

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += (float)xoffset;
	Pitch += (float)yoffset;

	if (constrainPitch) {
		if (Pitch > 89.0f) {
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f) {
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

/**
* 监听鼠标滚轮事件， 控制镜头缩放
*
*/
void Camera::ProcessMouseScroll(float yoffset) {

	Zoom -= (float)yoffset;
	if (Zoom <= 1.0f) {
		Zoom = 1.0f;
	}
	if (Zoom >= 45.0f) {
		Zoom = 45.0f;
	}
}

void Camera::updateCameraVectors() {

	glm::vec3 front;

	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));

	Front = glm::normalize(front);

	//normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
 