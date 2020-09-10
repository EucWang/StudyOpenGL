#ifndef  CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

/**
* 镜头移动方向
*/
enum class Camera_Movement {
	FORWARD,   //前
	BACKWARD,  //后
	LEFT,      //左
	RIGHT      //右
};

const float YAW = -90.0f;          // 俯仰角默认值

const float PITCH = 0.0f;          // 偏航角 默认值

const float SPEED = 2.5f;          // 键盘操作移动时，控制移动的速率

const float SENSITIVITY = 0.1f;    //鼠标控制移动时， 控制移动的速率

const float ZOOM = 45.0f;		   //鼠标滚轮控制时， 控制缩放的角度 ， 等同于 透视方法glm::perspective(glm::radians(fov), width/height, 0/1f. 100.0f); 这个方法中的fov

/**
* 镜头控制类
* 
* 在键盘控制移动； 鼠标控制移动，鼠标滚轮控制缩放时，设置对应的镜头 矩阵参数
* 
*  并可以通过此Camera类对象， 获取视角矩阵， 以及获取 透视方法的第一个参数
*/
class Camera {

public:
	
	//camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//euler Angles  欧拉角
	float Yaw;      //俯仰角
	float Pitch;    //偏航角


	//camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH);

	//constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


	//return the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	//processes input received from any keyboard-like unput sytstem.
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, double deltaTime);

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
	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true);

	/**
	* 监听鼠标滚轮事件， 控制镜头缩放
	* 
	*/
	void ProcessMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};


#endif // ! CAMEARA_H_