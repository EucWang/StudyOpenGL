#ifndef WINDOW_HELPER_H_
#define WINDOW_HELPER_H_

//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
#include  "camera.h"
#include <string>
#include <iostream>
#include "RenderUtil.h"

using namespace std;

#define SMALL_WINDOW_SCREEN_WIDTH 800
#define SMALL_WINDOW_SCREEN_HEIGHT 640

class WindowHelper {
public:
	WindowHelper(const char* title="OpenGL Windows",
		Camera camera=Camera(glm::vec3(0.0f, 1.0f, 4.0f)),
		int antiAliasing = 0,
		glm::vec3 bgColor= glm::vec3(0.2f, 0.3f, 0.3f),
		int width = SMALL_WINDOW_SCREEN_WIDTH,
		int height = SMALL_WINDOW_SCREEN_HEIGHT,
		int majorVersion = 3,
		int minorVersion = 3,
		bool resize = false
	);

	void create();

	GLFWwindow* getWindow();

	Camera getCamera();

	void run(void (*render)(void));

	/// <summary>
	/// ����processInput����ָ��
	/// ����, ʹ��calcProcessInput()��������,
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	void (*getProcessInput())(GLFWwindow*);

	/// <summary>
	/// �������ÿһ֡�ļ��ʱ��,
	/// Ȼ������ �������������(WSAD),���ڿ���Camera
	/// �� ��Ⱦ��whileѭ���е���
	/// </summary>
	void calcProcessInput();

private:
	string mTitle;
	int mAntiAliasing = 0;// �����,���ز����ĵ���
	int mScreenWidth;  //��Ļ���
	int mScreenHeight;
	int mMajorVersion = 3;
	int mMinorVersion = 3;
	bool mResize = false;
	glm::vec3 mBackgroundColor;
	GLFWwindow* mWindow;

	void calcDeltaTime();  //ÿһ֡��Ⱦʱ,�������һ֡��ʱ����

	//double deltaTime;  //��Ⱦ��֮֡���ʱ����
	//double lastFrame;  //��֡��Ⱦ��ʱ��

	//double lastX;  //��ʱ����
	//double lastY;

	//bool isMouseFirstIn = true;  //��ʱ����

	//void processInput(GLFWwindow* window);

	//void mouse_move_callback(GLFWwindow* window, double posX, double posY);

	//void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

	//void buffer_window_callback(GLFWwindow* window, int width, int height);
};

#endif // !WINDOW_HELPER_H_
