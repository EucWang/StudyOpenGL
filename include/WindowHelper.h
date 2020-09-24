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
	/// 返回processInput函数指针
	/// 废弃, 使用calcProcessInput()方法代替,
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	void (*getProcessInput())(GLFWwindow*);

	/// <summary>
	/// 计算绘制每一帧的间隔时间,
	/// 然后设置 输入的上下左右(WSAD),用于控制Camera
	/// 在 渲染的while循环中调用
	/// </summary>
	void calcProcessInput();

private:
	string mTitle;
	int mAntiAliasing = 0;// 抗锯齿,多重采样的点数
	int mScreenWidth;  //屏幕宽高
	int mScreenHeight;
	int mMajorVersion = 3;
	int mMinorVersion = 3;
	bool mResize = false;
	glm::vec3 mBackgroundColor;
	GLFWwindow* mWindow;

	void calcDeltaTime();  //每一帧渲染时,计算绘制一帧的时间间隔

	//double deltaTime;  //渲染两帧之间的时间间隔
	//double lastFrame;  //上帧渲染的时间

	//double lastX;  //临时变量
	//double lastY;

	//bool isMouseFirstIn = true;  //临时变量

	//void processInput(GLFWwindow* window);

	//void mouse_move_callback(GLFWwindow* window, double posX, double posY);

	//void mouse_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

	//void buffer_window_callback(GLFWwindow* window, int width, int height);
};

#endif // !WINDOW_HELPER_H_
