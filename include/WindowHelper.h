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
		Camera camera=Camera(glm::vec3(0.0f, 2.0f, 3.0f)),
		int antiAliasing = 4,
		//glm::vec3 bgColor= glm::vec3(0.2f, 0.3f, 0.3f),
		int width = SMALL_WINDOW_SCREEN_WIDTH,
		int height = SMALL_WINDOW_SCREEN_HEIGHT,
		int majorVersion = 3,
		int minorVersion = 3,
		bool resize = false
	);

	void create();

	GLFWwindow* getWindow();


	//TODO
	//void run(void (*render)(void));

	/// <summary>
	/// ����processInput����ָ��
	/// ����, ʹ��calcProcessInput()��������,
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static void (*getProcessInputFunc())(GLFWwindow*);

	static void(*getMouseMoveCallbackFunc())(GLFWwindow*, double, double);

	static void (*getMouseScrollCallbackFunc())(GLFWwindow*, double, double);

	static void(*getBufferWindowCallbackFunc())(GLFWwindow* window, int width, int height);

	static void calcDeltaTime(); //ÿһ֡��Ⱦʱ,�������һ֡��ʱ���� 

	static Camera getCamera();

	/// <summary>
	/// �������ÿһ֡�ļ��ʱ��,
	/// Ȼ������ �������������(WSAD),���ڿ���Camera
	/// �� ��Ⱦ��whileѭ���е���
	/// </summary>
	 void calcProcessInput();

	int getScreenWidth();

	int getScreenHeight();

private:
	string mTitle;
	int mAntiAliasing = 0;// �����,���ز����ĵ���
	int mScreenWidth;  //��Ļ���
	int mScreenHeight;
	int mMajorVersion = 3;
	int mMinorVersion = 3;
	bool mResize = false;
	GLFWwindow* mWindow;

};

#endif // !WINDOW_HELPER_H_
