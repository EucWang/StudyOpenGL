#ifndef  CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

/**
* ��ͷ�ƶ�����
*/
enum class Camera_Movement {
	FORWARD,   //ǰ
	BACKWARD,  //��
	LEFT,      //��
	RIGHT      //��
};

const float YAW = -90.0f;          // ������Ĭ��ֵ

const float PITCH = 0.0f;          // ƫ���� Ĭ��ֵ

const float SPEED = 2.5f;          // ���̲����ƶ�ʱ�������ƶ�������

const float SENSITIVITY = 0.001f;    //�������ƶ�ʱ�� �����ƶ�������

const float ZOOM = 45.0f;		   //�����ֿ���ʱ�� �������ŵĽǶ� �� ��ͬ�� ͸�ӷ���glm::perspective(glm::radians(fov), width/height, 0/1f. 100.0f); ��������е�fov

/**
* ��ͷ������
* 
* �ڼ��̿����ƶ��� �������ƶ��������ֿ�������ʱ�����ö�Ӧ�ľ�ͷ �������
* 
*  ������ͨ����Camera����� ��ȡ�ӽǾ��� �Լ���ȡ ͸�ӷ����ĵ�һ������
*/
class Camera {

public:
	
	//camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//euler Angles  ŷ����
	float Yaw;      //������
	float Pitch;    //ƫ����


	//camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f),
			float yaw = YAW, 
			float pitch = PITCH) 
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		  MovementSpeed(SPEED), 
		  MouseSensitivity(SENSITIVITY),
		  Zoom(ZOOM){

		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();
	}

	//constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM) {
	
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);

		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();

	}

	//return the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	//processes input received from any keyboard-like unput sytstem.
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, double deltaTime) {

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
	* ��������ƶ��¼�ʱ���ô˷����� ���ƾ�ͷ���ƶ�
	* 
	* ���ô˷���ǰ�� 
	* 1. ȷ�����û�ȡ��꣬���ع��ķ����� 
	*		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	* 2. ����ע������ƶ��¼��Ļص�����: 
	*		glfwSetCursorPosCallback(window, mouse_callback);
	* 3. ������ƶ��¼��Ļص������е��ô˺���
	*		void mouse_callback(GLFWwindow * window, double xpos, double ypos){  //... }
	*/
	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true) {
		
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += (float)xoffset;
		Pitch += (float)yoffset;

		if (constrainPitch) {
			if (Pitch > 89.0f){
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	/**
	* �����������¼��� ���ƾ�ͷ����
	* 
	*/
	void ProcessMouseScroll(float yoffset) {

		Zoom -= (float)yoffset;
		if (Zoom <= 1.0f) {
			Zoom = 1.0f;
		}
		if (Zoom >= 45.0f) {
			Zoom = 45.0f;
		}
	}

private:
	void updateCameraVectors() {
		
		glm::vec3 front;

		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Yaw));
		front.z = cos(glm::radians(Yaw)) * sin(glm::radians(Pitch));

		Front = glm::normalize(front);

		//normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

};


#endif // ! CAMEARA_H_