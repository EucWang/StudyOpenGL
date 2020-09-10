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

const float SENSITIVITY = 0.1f;    //�������ƶ�ʱ�� �����ƶ�������

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
	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true);

	/**
	* �����������¼��� ���ƾ�ͷ����
	* 
	*/
	void ProcessMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};


#endif // ! CAMEARA_H_